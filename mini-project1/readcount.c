#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"

int main() {
    int initial = getreadcount();
    printf("Initial readcount: %d\n", initial);
    
    int fd = open("README", O_RDONLY);
    char buf[100];
    read(fd, buf, 100);
    close(fd);
    
    int final = getreadcount();
    printf("Final readcount: %d\n", final);
    exit(0);
}
