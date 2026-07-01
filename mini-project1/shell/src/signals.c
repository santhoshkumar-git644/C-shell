#include "signals.h"
#include <signal.h>
#include <stdio.h>
#include <unistd.h>

void sigint_handler(int sig) {
    // Just ignore Ctrl-C in the parent shell
    printf("\n");
    // Should reprint the prompt here if needed, 
    // but the main loop handles it mostly.
}

void sigtstp_handler(int sig) {
    // Ignore Ctrl-Z in the parent shell
    printf("\n");
}

void setup_signal_handlers() {
    struct sigaction sa_int;
    sa_int.sa_handler = sigint_handler;
    sigemptyset(&sa_int.sa_mask);
    sa_int.sa_flags = SA_RESTART;
    sigaction(SIGINT, &sa_int, NULL);

    struct sigaction sa_tstp;
    sa_tstp.sa_handler = sigtstp_handler;
    sigemptyset(&sa_tstp.sa_mask);
    sa_tstp.sa_flags = SA_RESTART;
    sigaction(SIGTSTP, &sa_tstp, NULL);
}
