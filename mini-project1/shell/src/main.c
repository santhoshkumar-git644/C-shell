#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prompt.h"
#include "ast.h"
#include "parser.h"
#include "exec.h"

int main() {
    char home_dir[1024];
    if (getcwd(home_dir, sizeof(home_dir)) == NULL) {
        perror("getcwd");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    while (1) {
        display_prompt(home_dir);
        
        nread = getline(&line, &len, stdin);
        if (nread == -1) {
            // EOF (Ctrl-D)
            printf("\n");
            break;
        }

        // Parse the input
        ShellCmd *cmd = parse_command_line(line);
        if (!cmd) {
            continue;
        }

        execute_command(cmd);

        free_shell_cmd(cmd);
        free(cmd);
    }

    free(line);
    return 0;
}
