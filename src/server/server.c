#include "server.h"

void sig_handle_s(int signo)
{
    if(signo == SIGINT) //handle SIGINT
    {
        close(ss);  //close open socket
        for(int i=0;i<5;i++) if(scs[i]!=-1) close(scs[i]);
        puts("");   //to maintain terminal allignment
        exit(1);  //terminate process  
    }
}

void usage()
{
    printf("./chat-server [PORT-NUM]\n");   //show usage
}

int init_socket(struct sockaddr_in *sockaddr,char* port)
{
    char* pEnd = 0;
    errno = 0;
    uint16_t port_num = strtoul(port,&pEnd,10); //atoi can't detect error
    if(port == pEnd || errno == ERANGE || *pEnd!=0) return -1;  //strtoul failure
    sockaddr->sin_family = AF_INET; //IPv4
    sockaddr->sin_port = htons(port_num);   //transform port from host order to network order
    (sockaddr->sin_addr).s_addr = htonl(INADDR_ANY);    //0.0.0.0

    return 0;
}

void conn_succ_server(struct sockaddr_in* cs_addr)
{
    uint16_t port = ntohs(cs_addr->sin_port);   //transform port from network order to host order
    char* ip = inet_ntoa(cs_addr->sin_addr);    //transform IP from 32bit integer to dot-separated format
    printf("Connection from %s:%hu\n",ip,port); //server connection success message
}

void chat_server(int cs)
{
    uint8_t flag = 1;
    char send_buf[MAX_BUF_LEN] = {0};
    char recv_buf[MAX_BUF_LEN] = {0};
    
    while(flag)
    {
        flag = recv_msg(cs, recv_buf,sizeof(recv_buf)); //server receives the message first
        if(flag == 0)
        {
            printf("Disconnected\n");   //flag==0 means QUIT has received
            break;
        }

        flag = send_msg(cs,send_buf,sizeof(send_buf));  //client transmits the message after receiving is completed
        if(flag == 0)
        {
            printf("Disconnected\n");   //flag==0 means QUIT has transmitted
            break;
        }

        //initialize used buffer to prevent misusage of leftover information
        memset(recv_buf,0,MAX_BUF_LEN);
        memset(send_buf,0,MAX_BUF_LEN);
    }
}
