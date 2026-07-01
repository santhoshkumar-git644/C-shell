#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

extern char shell_home_dir[1024];

#define HISTORY_MAX 15

// Read history into array. Returns count.
int read_history(char history[HISTORY_MAX][1024]) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/.shell_history", shell_home_dir);
    FILE *f = fopen(path, "r");
    if (!f) return 0;
    
    int count = 0;
    while (count < HISTORY_MAX && fgets(history[count], 1024, f)) {
        // strip newline
        size_t len = strlen(history[count]);
        if (len > 0 && history[count][len-1] == '\n') {
            history[count][len-1] = '\0';
        }
        count++;
    }
    fclose(f);
    return count;
}

// Write history back
void write_history(char history[HISTORY_MAX][1024], int count) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/.shell_history", shell_home_dir);
    FILE *f = fopen(path, "w");
    if (!f) return;
    
    for (int i = 0; i < count; i++) {
        fprintf(f, "%s\n", history[i]);
    }
    fclose(f);
}

void add_to_log(const char *cmd) {
    // Skip if it contains "log" as an atomic command
    if (strstr(cmd, "log") != NULL) {
        return;
    }

    char history[HISTORY_MAX][1024];
    int count = read_history(history);

    // Skip if identical to previous
    if (count > 0 && strcmp(history[count - 1], cmd) == 0) {
        return;
    }

    if (count < HISTORY_MAX) {
        strncpy(history[count], cmd, 1024);
        count++;
    } else {
        // Shift left
        for (int i = 1; i < HISTORY_MAX; i++) {
            strncpy(history[i - 1], history[i], 1024);
        }
        strncpy(history[HISTORY_MAX - 1], cmd, 1024);
    }

    write_history(history, count);
}

// execute logic requires shell parsing, we will rely on main.c 
// passing the unparsed string.
void builtin_log(char **args, int arg_count, void (*execute_cb)(const char*)) {
    char history[HISTORY_MAX][1024];
    int count = read_history(history);

    if (arg_count == 0) {
        for (int i = 0; i < count; i++) {
            printf("%s\n", history[i]);
        }
    } else if (strcmp(args[0], "purge") == 0) {
        char path[1024];
        snprintf(path, sizeof(path), "%s/.shell_history", shell_home_dir);
        remove(path);
    } else if (strcmp(args[0], "execute") == 0 && arg_count == 2) {
        int index = atoi(args[1]);
        if (index > 0 && index <= count) {
            // Newest to oldest, 1-indexed. So index 1 is count - 1.
            int real_idx = count - index;
            if (execute_cb) {
                execute_cb(history[real_idx]);
            }
        } else {
            printf("log: Invalid index!\n");
        }
    } else {
        printf("log: Invalid Syntax!\n");
    }
}
