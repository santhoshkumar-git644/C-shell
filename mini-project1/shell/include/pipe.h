#ifndef PIPE_H
#define PIPE_H

#include "ast.h"
#include <stdbool.h>

// Execute a pipeline of commands.
void execute_pipeline(CmdGroup *group, bool run_bg);

#endif // PIPE_H
