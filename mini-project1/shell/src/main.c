#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "prompt.h"

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

        // Just echo for now, later we will parse
        // printf("Received: %s", line);
    }

    free(line);
    return 0;
}
