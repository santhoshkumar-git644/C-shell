#include "parser.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

// A tokenizer for parsing shell grammar rules.
// Handles ;, &, |, <, >, >> and tokens.
ShellCmd* parse_command_line(const char *line) {
    if (!line) return NULL;

    ShellCmd *cmd = calloc(1, sizeof(ShellCmd));
    if (!cmd) return NULL;

    const char *p = line;
    char token[1024];
    int t_idx = 0;

    int g_idx = 0;
    int a_idx = 0;
    
    // We implicitly start with an empty atomic cmd in a cmd group.
    AtomicCmd *current_atomic = &cmd->groups[g_idx].commands[a_idx];
    
    int expect_filename = 0; // 0=none, 1=<, 2=>, 3=>>

    while (*p) {
        // Skip whitespace
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;

        // Special characters
        if (strncmp(p, ">>", 2) == 0) {
            p += 2;
            if (expect_filename) goto syntax_error;
            expect_filename = 3;
            continue;
        } else if (*p == '>') {
            p++;
            if (expect_filename) goto syntax_error;
            expect_filename = 2;
            continue;
        } else if (*p == '<') {
            p++;
            if (expect_filename) goto syntax_error;
            expect_filename = 1;
            continue;
        } else if (*p == '|') {
            p++;
            if (expect_filename || current_atomic->arg_count == 0) goto syntax_error;
            a_idx++;
            if (a_idx >= MAX_ATOMIC) goto syntax_error;
            cmd->groups[g_idx].cmd_count = a_idx + 1;
            current_atomic = &cmd->groups[g_idx].commands[a_idx];
            continue;
        } else if (*p == ';' || *p == '&') {
            char op = *p++;
            if (expect_filename) goto syntax_error;
            
            if (current_atomic->arg_count == 0 && a_idx == 0) {
                // empty group, just skip if it's not the start, else error?
                // actually " ; cmd" is syntax error.
                if (g_idx == 0 && a_idx == 0 && current_atomic->arg_count == 0) goto syntax_error;
            } else {
                cmd->groups[g_idx].cmd_count = a_idx + 1;
            }

            if (op == '&') {
                cmd->run_bg[g_idx] = true;
            }
            
            g_idx++;
            if (g_idx >= MAX_GROUPS) goto syntax_error;
            cmd->group_count = g_idx;
            
            a_idx = 0;
            current_atomic = &cmd->groups[g_idx].commands[a_idx];
            continue;
        }

        // Read token
        t_idx = 0;
        while (*p && !isspace((unsigned char)*p) && *p != '|' && *p != '&' && *p != ';' && *p != '<' && *p != '>') {
            token[t_idx++] = *p++;
        }
        token[t_idx] = '\0';

        if (expect_filename) {
            if (expect_filename == 1) {
                current_atomic->redir.input_file = strdup(token);
            } else if (expect_filename == 2) {
                current_atomic->redir.output_file = strdup(token);
                current_atomic->redir.append_output = false;
            } else if (expect_filename == 3) {
                current_atomic->redir.output_file = strdup(token);
                current_atomic->redir.append_output = true;
            }
            expect_filename = 0;
        } else {
            if (current_atomic->arg_count < MAX_ARGS - 1) {
                current_atomic->args[current_atomic->arg_count++] = strdup(token);
            }
        }
    }

    if (expect_filename) goto syntax_error;

    if (current_atomic->arg_count > 0) {
        cmd->groups[g_idx].cmd_count = a_idx + 1;
        cmd->group_count = g_idx + 1;
    } else if (a_idx > 0) {
        // e.g. "cmd | " -> error
        goto syntax_error;
    } else {
        // e.g. "cmd ; " -> it's okay, just g_idx groups.
        cmd->group_count = g_idx;
    }

    if (cmd->group_count == 0) {
        free_shell_cmd(cmd);
        free(cmd);
        return NULL;
    }

    return cmd;

syntax_error:
    printf("Invalid Syntax!\n");
    free_shell_cmd(cmd);
    free(cmd);
    return NULL;
}
