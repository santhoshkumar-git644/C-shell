#ifndef REDIR_H
#define REDIR_H

#include "ast.h"

// Set up I/O redirection for the current process.
// Returns 0 on success, -1 on failure.
int setup_redirection(AtomicCmd *atomic);

#endif // REDIR_H
