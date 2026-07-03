#include "jobs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

Job job_list[MAX_JOBS];
int num_jobs = 0;

void add_job(pid_t pid, const char *command) {
    if (num_jobs < MAX_JOBS) {
        job_list[num_jobs].pid = pid;
        strncpy(job_list[num_jobs].command, command, 1023);
        job_list[num_jobs].state = 0; // Running
        job_list[num_jobs].job_id = num_jobs + 1;
        num_jobs++;
    }
}

void remove_job(pid_t pid) {
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].pid == pid) {
            for (int j = i; j < num_jobs - 1; j++) {
                job_list[j] = job_list[j + 1];
            }
            num_jobs--;
            break;
        }
    }
}

void check_background_jobs() {
    int status;
    pid_t pid;
    while ((pid = waitpid(-1, &status, WNOHANG | WUNTRACED)) > 0) {
        for (int i = 0; i < num_jobs; i++) {
            if (job_list[i].pid == pid) {
                if (WIFEXITED(status)) {
                    printf("\n%s with pid %d exited normally\n", job_list[i].command, pid);
                    remove_job(pid);
                } else if (WIFSIGNALED(status)) {
                    printf("\n%s with pid %d exited abnormally\n", job_list[i].command, pid);
                    remove_job(pid);
                } else if (WIFSTOPPED(status)) {
                    job_list[i].state = 1;
                    printf("\n[%d] Stopped %s\n", job_list[i].job_id, job_list[i].command);
                }
                break;
            }
        }
    }
}

void print_activities() {
    for (int i = 0; i < num_jobs; i++) {
        const char *state_str = (job_list[i].state == 0) ? "Running" : "Stopped";
        printf("[%d] %d %s - %s\n", job_list[i].job_id, job_list[i].pid, job_list[i].command, state_str);
    }
}

void ping_job(int job_id, int sig) {
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].job_id == job_id) {
            sig = sig % 32;
            if (kill(job_list[i].pid, sig) == 0) {
                printf("Sent signal %d to process with pid %d\n", sig, job_list[i].pid);
            } else {
                perror("kill");
            }
            return;
        }
    }
    printf("No such job found\n");
}

void builtin_activities(char **args, int arg_count) {
    print_activities();
}

void builtin_ping(char **args, int arg_count) {
    if (arg_count != 2) {
        printf("Usage: ping <job_id> <signal>\n");
        return;
    }
    int job_id = atoi(args[0]);
    int sig = atoi(args[1]);
    ping_job(job_id, sig);
}

void builtin_fg(char **args, int arg_count) {
    if (arg_count != 1) {
        printf("Usage: fg <job_id>\n");
        return;
    }
    int job_id = atoi(args[0]);
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].job_id == job_id) {
            pid_t pid = job_list[i].pid;
            
            // Bring to foreground
            printf("%s\n", job_list[i].command);
            kill(pid, SIGCONT);
            job_list[i].state = 0; // Running
            
            tcsetpgrp(STDIN_FILENO, pid);

            int status;
            waitpid(pid, &status, WUNTRACED);
            
            if (WIFEXITED(status) || WIFSIGNALED(status)) {
                remove_job(pid);
            } else if (WIFSTOPPED(status)) {
                job_list[i].state = 1; // Stopped
            }

            tcsetpgrp(STDIN_FILENO, getpgrp());
            return;
        }
    }
    printf("No such job\n");
}

void builtin_bg(char **args, int arg_count) {
    if (arg_count != 1) {
        printf("Usage: bg <job_id>\n");
        return;
    }
    int job_id = atoi(args[0]);
    for (int i = 0; i < num_jobs; i++) {
        if (job_list[i].job_id == job_id) {
            if (job_list[i].state == 0) {
                printf("Job already running\n");
                return;
            }
            kill(job_list[i].pid, SIGCONT);
            job_list[i].state = 0; // Running
            printf("[%d] %s &\n", job_list[i].job_id, job_list[i].command);
            return;
        }
    }
    printf("No such job\n");
}
