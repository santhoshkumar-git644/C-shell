#ifndef AST_H
#define AST_H

#include <stdbool.h>

#define MAX_ARGS 128
#define MAX_ATOMIC 64
#define MAX_GROUPS 64

// Struct for I/O redirection
typedef struct {
    char *input_file;
    char *output_file;
    bool append_output;
} RedirectInfo;

// Struct for a single atomic command
typedef struct {
    char *args[MAX_ARGS];
    int arg_count;
    RedirectInfo redir;
} AtomicCmd;

// Struct for a pipeline of atomic commands
typedef struct {
    AtomicCmd commands[MAX_ATOMIC];
    int cmd_count;
} CmdGroup;

// Struct for the entire shell command line
typedef struct {
    CmdGroup groups[MAX_GROUPS];
    bool run_bg[MAX_GROUPS]; // True if this group ends with '&'
    int group_count;
} ShellCmd;

void free_shell_cmd(ShellCmd *cmd);

#endif // AST_H
