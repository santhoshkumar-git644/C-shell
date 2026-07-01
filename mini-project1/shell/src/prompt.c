#include "prompt.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void display_prompt(const char *home_dir) {
    char username[256];
    char hostname[256];
    char cwd[1024];

    if (getlogin_r(username, sizeof(username)) != 0) {
        strncpy(username, "unknown", sizeof(username));
    }

    if (gethostname(hostname, sizeof(hostname)) != 0) {
        strncpy(hostname, "unknown", sizeof(hostname));
    }

    if (getcwd(cwd, sizeof(cwd)) == NULL) {
        strncpy(cwd, "unknown", sizeof(cwd));
    }

    // Replace home directory prefix with '~'
    char display_cwd[1024];
    size_t home_len = strlen(home_dir);

    if (strncmp(cwd, home_dir, home_len) == 0 && (cwd[home_len] == '\0' || cwd[home_len] == '/')) {
        snprintf(display_cwd, sizeof(display_cwd), "~%s", cwd + home_len);
    } else {
        strncpy(display_cwd, cwd, sizeof(display_cwd));
    }

    printf("<%s@%s:%s> ", username, hostname, display_cwd);
    fflush(stdout);
}
