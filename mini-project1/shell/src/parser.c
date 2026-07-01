#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// A basic tokenizer for parsing. In a real scenario, this would handle
// quotes and all grammar rules fully.
ShellCmd* parse_command_line(const char *line) {
    if (!line) return NULL;

    ShellCmd *cmd = calloc(1, sizeof(ShellCmd));
    if (!cmd) return NULL;

    // TODO: implement full CFG parsing.
    // For now, this is just a stub that will parse a single simple command
    // so we can execute something.
    char *copy = strdup(line);
    char *saveptr;
    char *token = strtok_r(copy, " \t\n\r", &saveptr);
    
    if (token) {
        cmd->group_count = 1;
        cmd->groups[0].cmd_count = 1;
        AtomicCmd *atomic = &cmd->groups[0].commands[0];
        
        while (token && atomic->arg_count < MAX_ARGS - 1) {
            atomic->args[atomic->arg_count++] = strdup(token);
            token = strtok_r(NULL, " \t\n\r", &saveptr);
        }
        atomic->args[atomic->arg_count] = NULL;
    }

    free(copy);

    if (cmd->group_count == 0) {
        free(cmd);
        return NULL;
    }

    return cmd;
}
