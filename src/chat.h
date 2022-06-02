#pragma once

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <limits.h>
#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define MAX_BUF_LEN 200 //max buffer size during chatting

/*
    function to handle error
    close open socket when error occurs
*/
void handleErr(int fd,const char const* msg);

/*
    function to receive messages
*/
int recv_msg(int fd, char* buf, int maxlen);

/*
    function to transmit messages
*/
int send_msg(int fd, char* buf, int maxlen);