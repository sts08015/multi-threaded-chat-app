#pragma once

#include "../chat.h"

#define BACKLOG 5   //size of backlog used in listen function
#define CAPACITY 5  //maximum client connection capacity
#define BUFLEN 222  //maximum length of buffer used in server

/*
    parameter used in thread per client
*/
typedef struct thread_param
{
    int idx;
    struct sockaddr_in cs_addr;
}TP;

extern pthread_mutex_t mutex; //mutex used in sever

extern volatile int cnt; //total connected client num
extern int ss;  //server's welcoming socket
extern int* scs;    //server's opened sockets

/*
    function to close open socket when SIGINT occurs
*/
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

/*
    function to broadcast received message to connected clients except sender
*/
void broadcast(int idx,char* str,int len);

/*
    thread per connected client
*/
void thread_main(void* param);