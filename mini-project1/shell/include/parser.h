#ifndef PARSER_H
#define PARSER_H

#include "ast.h"

// Parse a line of input into a ShellCmd structure.
// Returns a pointer to a dynamically allocated ShellCmd if successful.
// Returns NULL on syntax error.
ShellCmd* parse_command_line(const char *line);

#endif // PARSER_H
