#include "common.h"

int lowercase(char* buf) {
    char* c;
    for (c = buf; *c; ++c) {
        *c = *c > 0x40 && *c < 0x5b ? *c | 0x60 : *c;
    }
    return c - buf;
}

int lowercase_cpy(char* dst, const char* src) {
    const char* c;
    for (c = src; *c; ++c) {
        dst[c - src] = *c > 0x40 && *c < 0x5b ? *c | 0x60 : *c;
    }
    dst[c - src] = 0;
    return c - src;
}

int get_line(int sockfd, char* buf, int len) {
    int i = 0;
    char c = 0;

    while (i < len - 1) {
        int n = recv(sockfd, &c, 1, 0);
        if (n > 0) {
            buf[i] = c;
            ++i;
            if (c == '\n') {
                break;
            }
        } else {
            break;
        }
    }
    buf[i] = 0;

    return i;
}

void send_header(int sockfd, const char* key, const char* value) {
    char* msg = (char*)malloc(1024);
    sprintf(msg, "%s: %s\r\n", key, value);
    send(sockfd, msg, strlen(msg), 0);
    free(msg);
}

void send_body(int sockfd, const const char* body, int len) {
    if (len > 0) {
        char* value = (char*)malloc(64);
        sprintf(value, "%d", len);
        send_header(sockfd, CONTENT_LENGTH, value);
        free(value);
    }
    send_header(sockfd, CONNECTION, "close");
    send(sockfd, "\r\n", 2, 0);
    int sent = 0;
    int offset = 0;
    while (offset < len && (sent = send(sockfd, &body[offset], len - offset, 0)) >= 0) {
        offset += sent;
    }

    #ifdef TCP_CORK
        if (setsockopt(sockfd, SOL_TCP, TCP_CORK, &(int){ 0 }, sizeof(int)) < 0) {
            iprintf("[!] Could not unset cork option!");
        }
    #endif
}
