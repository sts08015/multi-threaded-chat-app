#include "client.h"

int cs; //socket file descriptor for client
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
volatile int chk = 1;

int main(int argc, char* argv[])
{
    if(argc!=4 || strlen(argv[3])>20)
    {
        usage();    //check IP and port have given as arguments
        return -1;
    }

    signal(SIGINT,sig_handle_c);    //assign SIGINT handler

    cs = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);   //open socket
    if(cs < 0)  //socket creation faliure
    {
        perror("socket creation error");    //print error message
        return -1;
    }
    
    struct sockaddr_in ss_addr = {0};
    int ret = init_socket(&ss_addr,argv[1],argv[2]);    //initialize server's info
    if(ret<0)   //initializing failed
    {
        handleErr(cs, "socket init error - client");    //print err msg and close socket
        return -1;
    }

    ret = connect(cs,(struct sockaddr*) &ss_addr,sizeof(ss_addr));  //make connection with server
    if(ret<0)   //connection failed
    {
        handleErr(cs,"connect error");  //print err msg and close socket
        return -1;
    }
    
    conn_succ_client(); //print connection success string
    chat_client(argv[3]);  //chat with server

    close(cs);  //close socket after chatting is over
    return 0;
}