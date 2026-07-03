#include "exec.h"
#include "builtin.h"
#include "pipe.h"
#include "redir.h"
#include "jobs.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <string.h>

extern void execute_line(const char *line);

void execute_single_atomic(AtomicCmd *atomic) {
    if (!atomic || atomic->arg_count == 0) exit(0);

    char *command = atomic->args[0];

    if (strcmp(command, "hop") == 0) {
        builtin_hop(&atomic->args[1], atomic->arg_count - 1);
        exit(0);
    } else if (strcmp(command, "reveal") == 0) {
        builtin_reveal(&atomic->args[1], atomic->arg_count - 1);
        exit(0);
    } else if (strcmp(command, "log") == 0) {
        builtin_log(&atomic->args[1], atomic->arg_count - 1, execute_line);
        exit(0);
    } else if (strcmp(command, "activities") == 0) {
        builtin_activities(&atomic->args[1], atomic->arg_count - 1);
        exit(0);
    } else if (strcmp(command, "ping") == 0) {
        builtin_ping(&atomic->args[1], atomic->arg_count - 1);
        exit(0);
    } else if (strcmp(command, "fg") == 0) {
        builtin_fg(&atomic->args[1], atomic->arg_count - 1);
        exit(0);
    } else if (strcmp(command, "bg") == 0) {
        builtin_bg(&atomic->args[1], atomic->arg_count - 1);
        exit(0);
    }

    if (execvp(atomic->args[0], atomic->args) == -1) {
        printf("Command not found!\n");
        exit(1);
    }
}

void execute_command(ShellCmd *cmd) {
    if (!cmd || cmd->group_count == 0) return;

    for (int i = 0; i < cmd->group_count; i++) {
        CmdGroup *group = &cmd->groups[i];
        if (group->cmd_count == 0) continue;
        
        bool run_bg = cmd->run_bg[i];

        // If it's a single built-in and not piped, execute in parent to affect shell state
        if (group->cmd_count == 1) {
            AtomicCmd *atomic = &group->commands[0];
            if (atomic->arg_count > 0) {
                char *c = atomic->args[0];
                if (strcmp(c, "hop") == 0) {
                    builtin_hop(&atomic->args[1], atomic->arg_count - 1);
                    continue;
                } else if (strcmp(c, "log") == 0) {
                    builtin_log(&atomic->args[1], atomic->arg_count - 1, execute_line);
                    continue;
                } else if (strcmp(c, "activities") == 0) {
                    builtin_activities(&atomic->args[1], atomic->arg_count - 1);
                    continue;
                } else if (strcmp(c, "ping") == 0) {
                    builtin_ping(&atomic->args[1], atomic->arg_count - 1);
                    continue;
                } else if (strcmp(c, "fg") == 0) {
                    builtin_fg(&atomic->args[1], atomic->arg_count - 1);
                    continue;
                } else if (strcmp(c, "bg") == 0) {
                    builtin_bg(&atomic->args[1], atomic->arg_count - 1);
                    continue;
                }
            }
        }

        execute_pipeline(group, run_bg);
    }
}
