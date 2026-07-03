#include "pipe.h"
#include "redir.h"
#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

extern void execute_single_atomic(AtomicCmd *atomic);

void execute_pipeline(CmdGroup *group, bool run_bg) {
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

            // Set child process group id so that terminal signals can be sent appropriately
            setpgid(0, 0);

            // Setup I/O redirection (this overrides pipes if specified)
            if (setup_redirection(&group->commands[i]) < 0) {
                exit(1);
            }

            AtomicCmd *atomic = &group->commands[i];
            char *command = atomic->args[0];
            if (!command) exit(0);

            execute_single_atomic(atomic); 
            exit(0); // fallback
        }
    }

    // Parent closes all pipe fds
    for (int i = 0; i < 2 * (num_cmds - 1); i++) {
        close(pipefds[i]);
    }

    // Give the terminal control to the child process group (the first process in the pipeline)
    if (!run_bg) {
        tcsetpgrp(STDIN_FILENO, pids[0]);
    }

    // Wait for children
    for (int i = 0; i < num_cmds; i++) {
        if (run_bg) {
            // Just add the last job in pipeline (or all jobs) to jobs list.
            if (i == num_cmds - 1) {
                char cmd_name[1024] = "";
                for (int c = 0; c < num_cmds; c++) {
                    if (c > 0) strncat(cmd_name, " | ", 1023 - strlen(cmd_name));
                    strncat(cmd_name, group->commands[c].args[0], 1023 - strlen(cmd_name));
                }
                add_job(pids[i], cmd_name);
                printf("[%d] %d\n", num_jobs, pids[i]);
            }
        } else {
            int status;
            waitpid(pids[i], &status, WUNTRACED);
            
            if (WIFSTOPPED(status)) {
                if (i == num_cmds - 1) {
                    char cmd_name[1024] = "";
                    for (int c = 0; c < num_cmds; c++) {
                        if (c > 0) strncat(cmd_name, " | ", 1023 - strlen(cmd_name));
                        strncat(cmd_name, group->commands[c].args[0], 1023 - strlen(cmd_name));
                    }
                    add_job(pids[i], cmd_name);
                    extern Job job_list[];
                    extern int num_jobs;
                    job_list[num_jobs - 1].state = 1; // stopped
                    printf("\n[%d] Stopped %s\n", job_list[num_jobs - 1].job_id, cmd_name);
                }
            }
        }
    }

    // Reclaim terminal control to the shell
    if (!run_bg) {
        tcsetpgrp(STDIN_FILENO, getpgrp());
    }
}
