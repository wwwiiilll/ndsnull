#ifndef COMMON_H
#define COMMON_H

#ifdef NDS
    #include <nds.h>
#else
    #include <stdlib.h>
    #include <unistd.h>
    #include <arpa/inet.h>
#endif
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>

#include "http.h"

#ifndef NDS
    #define iprintf(...) printf(__VA_ARGS__)
    #define closesocket(fd) close(fd)
#endif

int lowercase(char* buf);
int lowercase_cpy(char* dst, const char* src);

int get_line(int sockfd, char* buf, int len);

void send_header(int sockfd, const char* key, const char* value);
void send_body(int sockfd, const char* body, int len);

#endif
