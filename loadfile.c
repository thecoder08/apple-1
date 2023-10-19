#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "loadfile.h"

int loadFile(const char* filename, void* buffer) {
    int fd = open(filename, 0);
    if (fd == -1) {
        return 1;
    }
    struct stat st;
    fstat(fd, &st);
    read(fd, buffer, st.st_size);
    close(fd);
    return 0;
}