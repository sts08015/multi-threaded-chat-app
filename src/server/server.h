#pragma once

#include "../chat.h"

#define BACKLOG 5   //size of backlog used in listen function
#define CAPACITY 5
#define BUFLEN 222

typedef struct thread_param
{
    int idx;
    struct sockaddr_in cs_addr;
}TP;

extern pthread_mutex_t mutex; //mutex used in sever

extern int ss;  //global variable assigned at main.c
/*
    function to close open socket when SIGINT occurs
*/
extern int* scs;

void sig_handle_s(int signo);
/*
    function to print correct usage of the program when the user gave wrong arguments
*/
void usage();
/*
    function to initialize struct sockaddr_in
*/
int init_socket(struct sockaddr_in *sockaddr,char* port);
/*
    function to print the connection success message of the server
*/
void conn_succ_server(struct sockaddr_in* cs_addr);

void broadcast(int idx,char* str,int len);

void thread_main(void* param);
