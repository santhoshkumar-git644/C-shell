#ifndef BUILTIN_H
#define BUILTIN_H

void builtin_hop(char **args, int arg_count);
void builtin_reveal(char **args, int arg_count);
void builtin_log(char **args, int arg_count, void (*execute_cb)(const char*));
void add_to_log(const char *cmd);

#endif // BUILTIN_H
