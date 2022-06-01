#pragma once
#include "../chat.h"

extern int cs;  //global variable assigned at main.c
extern pthread_mutex_t mutex;
extern int chk;

/*
    function to close open socket when SIGINT occurs
*/
void sig_handle_c(int signo);
/*
    function to print correct usage of the program when the user gave wrong arguments
*/
void usage();
/*
    function to initialize struct sockaddr_in
*/
int init_socket(struct sockaddr_in *sockaddr,char* ip, char* port);
/*
    function to print the connection success message of the client
*/
void conn_succ_client();
/*
    function that implemented the chatting sequence of the client
*/
void chat_client(char* str);