#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>

extern char shell_home_dir[1024];
extern char prev_dir[1024];

int compare_strings(const void *a, const void *b) {
    return strcmp(*(const char **)a, *(const char **)b);
}

void builtin_reveal(char **args, int arg_count) {
    bool flag_a = false;
    bool flag_l = false;
    char target_dir[1024] = "."; // Default to current directory

    int i = 0;
    while (i < arg_count && args[i][0] == '-') {
        // Check if it is the previous directory argument "-"
        if (strcmp(args[i], "-") == 0) {
            break;
        }

        // Parse flags like -a, -l, -al
        for (size_t j = 1; j < strlen(args[i]); j++) {
            if (args[i][j] == 'a') {
                flag_a = true;
            } else if (args[i][j] == 'l') {
                flag_l = true;
            } else {
                printf("reveal: Invalid Syntax!\n");
                return;
            }
        }
        i++;
    }

    // Now parse the optional directory argument
    if (i < arg_count) {
        if (arg_count - i > 1) {
            printf("reveal: Invalid Syntax!\n");
            return;
        }
        
        char *arg = args[i];
        if (strcmp(arg, "~") == 0) {
            strncpy(target_dir, shell_home_dir, sizeof(target_dir));
        } else if (strcmp(arg, "-") == 0) {
            if (strlen(prev_dir) == 0) {
                printf("No such directory!\n");
                return;
            }
            strncpy(target_dir, prev_dir, sizeof(target_dir));
        } else {
            strncpy(target_dir, arg, sizeof(target_dir));
        }
    }

    DIR *dir = opendir(target_dir);
    if (!dir) {
        printf("No such directory!\n");
        return;
    }

    struct dirent *entry;
    char *entries[1024];
    int count = 0;

    while ((entry = readdir(dir)) != NULL) {
        if (!flag_a && entry->d_name[0] == '.') {
            continue;
        }
        entries[count++] = strdup(entry->d_name);
    }
    closedir(dir);

    qsort(entries, count, sizeof(char *), compare_strings);

    for (int j = 0; j < count; j++) {
        if (flag_l) {
            // Simplified details for the sake of the exercise
            printf("%s\n", entries[j]);
        } else {
            printf("%s ", entries[j]);
        }
        free(entries[j]);
    }

    if (!flag_l) {
        printf("\n");
    }
}
