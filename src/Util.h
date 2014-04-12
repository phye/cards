#include <stdio.h>

//Reliable write
int sock_write(int fd, char* buf, size_t sz);

//Reliable read
int sock_read(int fd, char* buf, size_t buf_sz);
