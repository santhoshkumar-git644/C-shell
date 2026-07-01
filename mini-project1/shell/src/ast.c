#include "ast.h"
#include <stdlib.h>

void free_shell_cmd(ShellCmd *cmd) {
    if (!cmd) return;
    for (int i = 0; i < cmd->group_count; ++i) {
        for (int j = 0; j < cmd->groups[i].cmd_count; ++j) {
            AtomicCmd *atomic = &cmd->groups[i].commands[j];
            for (int k = 0; k < atomic->arg_count; ++k) {
                free(atomic->args[k]);
                atomic->args[k] = NULL;
            }
            if (atomic->redir.input_file) {
                free(atomic->redir.input_file);
                atomic->redir.input_file = NULL;
            }
            if (atomic->redir.output_file) {
                free(atomic->redir.output_file);
                atomic->redir.output_file = NULL;
            }
        }
    }
}
