#include "pipe.h"
#include "redir.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include "builtin.h" // Wait, builtins in pipes get complicated.
// For now, let's keep it simple and just do execvp.
// If it's a builtin, we would technically run it in the child process
// but since builtins like 'hop' modify shell state, they usually don't make sense in pipes.
// Or we do handle them. We will just use a helper from exec.c.
extern void execute_single_atomic(AtomicCmd *atomic);

void execute_pipeline(CmdGroup *group) {
    if (group->cmd_count == 0) return;

    int num_cmds = group->cmd_count;
    int pipefds[2 * num_cmds];

    // Create pipes
    for (int i = 0; i < num_cmds - 1; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe");
            return;
        }
    }

    pid_t pids[num_cmds];

    for (int i = 0; i < num_cmds; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork");
            return;
        }

        if (pids[i] == 0) {
            // Child
            // Hook up STDIN if not first command
            if (i > 0) {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) < 0) {
                    perror("dup2 stdin");
                    exit(1);
                }
            }
            // Hook up STDOUT if not last command
            if (i < num_cmds - 1) {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) < 0) {
                    perror("dup2 stdout");
                    exit(1);
                }
            }

            // Close all pipe fds in child
            for (int j = 0; j < 2 * (num_cmds - 1); j++) {
                close(pipefds[j]);
            }

            // Setup I/O redirection (this overrides pipes if specified)
            if (setup_redirection(&group->commands[i]) < 0) {
                exit(1);
            }

            AtomicCmd *atomic = &group->commands[i];
            char *command = atomic->args[0];
            if (!command) exit(0);

            // Execute
            execute_single_atomic(atomic); // This will handle builtins or execvp, and exit(1) on fail.
            exit(0);
        }
    }

    // Parent closes all pipe fds
    for (int i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipefds[i]);
    }

    // Wait for all children
    for (int i = 0; i < num_cmds; i++) {
        int status;
        waitpid(pids[i], &status, 0);
    }
}
