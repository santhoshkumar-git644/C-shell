#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prompt.h"
#include "ast.h"
#include "parser.h"
#include "exec.h"
#include "builtin.h"
#include "signals.h"
#include "jobs.h"

char shell_home_dir[1024];
char prev_dir[1024] = "";

void execute_line(const char *line) {
    ShellCmd *cmd = parse_command_line(line);
    if (!cmd) {
        return;
    }
    execute_command(cmd);
    free_shell_cmd(cmd);
    free(cmd);
}

int main() {
    setup_signal_handlers();

    if (getcwd(shell_home_dir, sizeof(shell_home_dir)) == NULL) {
        perror("getcwd");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1) {
        check_background_jobs();
        display_prompt(shell_home_dir);
        
        nread = getline(&line, &len, stdin);
        if (nread == -1) {
            // EOF (Ctrl-D)
            printf("\nlogout\n");
            break;
        }

        // strip newline for log
        if (nread > 0 && line[nread-1] == '\n') {
            line[nread-1] = '\0';
        }
        
        if (strlen(line) > 0) {
            add_to_log(line);
        }

        execute_line(line);
    }

    free(line);
    return 0;
}
