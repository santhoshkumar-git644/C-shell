#include "redir.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int setup_redirection(AtomicCmd *atomic) {
    if (atomic->redir.input_file) {
        int fd_in = open(atomic->redir.input_file, O_RDONLY);
        if (fd_in < 0) {
            printf("No such file or directory\n");
            return -1;
        }
        if (dup2(fd_in, STDIN_FILENO) < 0) {
            perror("dup2 input");
            close(fd_in);
            return -1;
        }
        close(fd_in);
    }

    if (atomic->redir.output_file) {
        int flags = O_WRONLY | O_CREAT;
        if (atomic->redir.append_output) {
            flags |= O_APPEND;
        } else {
            flags |= O_TRUNC;
        }

        // Mode 0644
        int fd_out = open(atomic->redir.output_file, flags, 0644);
        if (fd_out < 0) {
            printf("Unable to create file for writing\n");
            return -1;
        }
        if (dup2(fd_out, STDOUT_FILENO) < 0) {
            perror("dup2 output");
            close(fd_out);
            return -1;
        }
        close(fd_out);
    }
    return 0;
}
