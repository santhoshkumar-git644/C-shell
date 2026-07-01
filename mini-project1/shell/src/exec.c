#include "exec.h"
#include "builtin.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

extern void execute_line(const char *line);

void execute_command(ShellCmd *cmd) {
    if (!cmd || cmd->group_count == 0) return;

    for (int i = 0; i < cmd->group_count; i++) {
        CmdGroup *group = &cmd->groups[i];
        if (group->cmd_count == 0) continue;

        AtomicCmd *atomic = &group->commands[0]; // Just execute first for now
        if (atomic->arg_count == 0) continue;

        char *command = atomic->args[0];

        if (strcmp(command, "hop") == 0) {
            builtin_hop(&atomic->args[1], atomic->arg_count - 1);
            continue;
        } else if (strcmp(command, "reveal") == 0) {
            builtin_reveal(&atomic->args[1], atomic->arg_count - 1);
            continue;
        } else if (strcmp(command, "log") == 0) {
            builtin_log(&atomic->args[1], atomic->arg_count - 1, execute_line);
            continue;
        }

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            // Child process
            if (execvp(atomic->args[0], atomic->args) == -1) {
                printf("Command not found!\n");
                exit(1);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
}
