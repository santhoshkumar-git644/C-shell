#ifndef JOBS_H
#define JOBS_H

#include "ast.h"
#include <sys/types.h>

#define MAX_JOBS 100

typedef struct {
    pid_t pid;
    char command[1024];
    int state; // 0 = Running, 1 = Stopped
    int job_id;
} Job;

extern Job job_list[MAX_JOBS];
extern int num_jobs;

// Job list management
void add_job(pid_t pid, const char *command);
void remove_job(pid_t pid);
void check_background_jobs();
void print_activities();
void ping_job(int job_id, int signal);

// Built-ins
void builtin_activities(char **args, int arg_count);
void builtin_ping(char **args, int arg_count);
void builtin_fg(char **args, int arg_count);
void builtin_bg(char **args, int arg_count);

#endif // JOBS_H
