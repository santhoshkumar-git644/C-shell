#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern char shell_home_dir[1024];
extern char prev_dir[1024];

void builtin_hop(char **args, int arg_count) {
    if (arg_count == 0) {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            strncpy(prev_dir, cwd, sizeof(prev_dir));
        }
        if (chdir(shell_home_dir) != 0) {
            printf("No such directory!\n");
        }
        return;
    }

    for (int i = 0; i < arg_count; i++) {
        char *arg = args[i];
        char cwd[1024];
        char target_dir[1024];

        if (strcmp(arg, "~") == 0) {
            strncpy(target_dir, shell_home_dir, sizeof(target_dir));
        } else if (strcmp(arg, ".") == 0) {
            continue; // Do nothing
        } else if (strcmp(arg, "..") == 0) {
            strncpy(target_dir, "..", sizeof(target_dir));
        } else if (strcmp(arg, "-") == 0) {
            if (strlen(prev_dir) == 0) {
                // Do nothing if no previous dir
                continue; 
            }
            strncpy(target_dir, prev_dir, sizeof(target_dir));
        } else {
            strncpy(target_dir, arg, sizeof(target_dir));
        }

        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            strncpy(prev_dir, cwd, sizeof(prev_dir));
        }

        if (chdir(target_dir) != 0) {
            printf("No such directory!\n");
            // If chdir fails, we should restore prev_dir to what it actually was?
            // "If the directory does not exist, output "No such directory!""
        }
    }
}
