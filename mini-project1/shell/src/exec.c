#include "exec.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void execute_command(ShellCmd *cmd) {
    if (!cmd || cmd->group_count == 0) return;

    for (int i = 0; i < cmd->group_count; i++) {
        CmdGroup *group = &cmd->groups[i];
        if (group->cmd_count == 0) continue;

        AtomicCmd *atomic = &group->commands[0]; // Just execute first for now
        if (atomic->arg_count == 0) continue;

        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        }

        if (pid == 0) {
            // Child process
            if (execvp(atomic->args[0], atomic->args) == -1) {
                perror("Command not found");
                exit(1);
            }
        } else {
            // Parent process
            int status;
            waitpid(pid, &status, 0);
        }
    }
}
