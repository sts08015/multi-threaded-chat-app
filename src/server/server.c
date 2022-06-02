#include "server.h"

void sig_handle_s(int signo)
{
    if(signo == SIGINT) //handle SIGINT
    {
        close(ss);  //close open socket
        pthread_mutex_lock(&mutex); //due to critical section
        for(int i=0;i<CAPACITY;i++) if(scs[i]!=-1) close(scs[i]);
        free(scs);
        pthread_mutex_unlock(&mutex);
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

void broadcast(int idx,char* str,int len)
{
    pthread_mutex_lock(&mutex); //due to critical section
    for(int i=0;i<CAPACITY;i++) //broadcast except sender
    {
        if(i==idx || scs[i]<0) continue;
        send(scs[i],str,len,0);
    }
    pthread_mutex_unlock(&mutex);
}

void thread_main(void* param)
{
    char nickname[BUFLEN] = {0};    //buffer to store nickname
    char buf[BUFLEN] = {0}; //
    TP p = *(TP*)param;
    conn_succ_server(&(p.cs_addr)); //print connection success string
    recv(scs[p.idx],nickname,BUFLEN,0); //receive nickname
    
    snprintf(buf,BUFLEN,"%s is connected",nickname);
    int len = strlen(buf);
    broadcast(p.idx,buf,len);   //broadcast connection message of new client
    puts(buf);
    
    uint8_t flag = 1;
    while(1)
    {
        memset(buf,0,BUFLEN);
        flag = recv_msg(scs[p.idx],buf,BUFLEN);
        if(flag == 0)   //if QUIT is received, broadcast disconnection announcement
        {
            snprintf(buf,BUFLEN,"%s is disconnected",nickname);
        }
        else    //broadcast message as nickname: msg format 
        {
            char* tmp = strdup(buf);
            snprintf(buf,BUFLEN,"%s: %s",nickname,tmp);
            free(tmp);
        }
        broadcast(p.idx,buf,strlen(buf));
        puts(buf);
        if(flag == 0)   //if QUIT is received, terminate thread
        {
            pthread_mutex_lock(&mutex); //due to critical section
            close(scs[p.idx]);
            scs[p.idx] = -1;
            pthread_mutex_unlock(&mutex);
            break;
        }
    }
}