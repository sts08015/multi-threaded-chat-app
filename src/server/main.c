#include "server.h"

int ss; //socket file descriptor for server
int scs[CAPACITY] = {-1,-1,-1,-1,-1};
int cnt;
pthread_t tid[CAPACITY];
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

typedef struct thread_param
{
    int idx;
    struct sockaddr_in cs_addr;
}TP;

void broadcast(int fd,char* str,int len)
{
    pthread_mutex_lock(&mutex);
    for(int i=0;i<CAPACITY;i++)
    {
        printf("%d ",scs[i]);
        if(i==fd || scs[i]<0) continue;
        send(scs[i],str,len,0);
    }
    pthread_mutex_unlock(&mutex);
    printf("fd : %d\n",fd);
}

void thread_main(void* param)
{
    char nickname[BUFLEN] = {0};
    char buf[BUFLEN] = {0};
    TP* p = (TP*)param;
    conn_succ_server(&(p->cs_addr)); //print connection success string
    recv(scs[p->idx],nickname,BUFLEN,0); //receive nickname
    
    snprintf(buf,BUFLEN,"%s is connected",nickname);
    int len = strlen(buf);
    broadcast(p->idx,buf,len);
    puts(buf);
    
    uint8_t flag = 1;
    while(1)
    {
        memset(buf,0,BUFLEN);
        flag = recv_msg(scs[p->idx],buf,BUFLEN);
        if(flag == 0)
        {
            snprintf(buf,BUFLEN,"%s is disconnected",nickname);
        }
        else
        {
            char* tmp = strdup(buf);
            snprintf(buf,BUFLEN,"%s:%s",nickname,tmp);
            free(tmp);
        }
        broadcast(p->idx,buf,strlen(buf));
        puts(buf);
        if(flag == 0)
        {
            pthread_mutex_lock(&mutex);
            close(scs[p->idx]);
            scs[p->idx] = -1;
            pthread_mutex_unlock(&mutex);
        }
    }
}

int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        usage();    //check port has given as an argument
        return -1;
    }

    signal(SIGINT,sig_handle_s);    //assign SIGINT handler

    ss = socket(PF_INET,SOCK_STREAM,IPPROTO_TCP);   //open welcome socket
    if(ss < 0)  //socket creation faliure
    {
        perror("socket creation error");    //print error message
        return -1;
    }

    struct sockaddr_in ss_addr = {0};
    int ret = init_socket(&ss_addr,argv[1]);    //initialize server's info
    if(ret < 0) //initializing failed
    {
        handleErr(ss,"socket init error - server"); //print err msg and close socket
        return -1;
    }
    
    ret = bind(ss, (struct sockaddr*) &ss_addr, sizeof(ss_addr));   //allocate IP address and port number
    if(ret < 0) //binding falied
    {
        handleErr(ss,"BindErr");    //print err msg and close socket
        return -1;
    }

    ret = listen(ss,BACKLOG);   //make socket passive and listen to connection requests
    if(ret<0)   //listen failed
    {
        handleErr(ss,"ListenErr");  //print err msg and close socket
        return -1;
    }

    struct sockaddr_in cs_addr = {0};
    socklen_t cs_addr_len = sizeof(cs_addr);
    TP tp = {0};
    while(1)
    {
        ret = accept(ss,(struct sockaddr*) &cs_addr,&cs_addr_len);  //accept connection
        if(ret<0 || cnt>=CAPACITY) continue;
        
        pthread_mutex_lock(&mutex);
        cnt++;
        for(int i=0;i<CAPACITY;i++)
        {
            if(scs[i]==-1)
            {
                tp.idx = i;
                scs[i] = ret;
                break;
            }
        }
        pthread_mutex_unlock(&mutex);

        tp.cs_addr = cs_addr;
        pthread_create(tid+tp.idx, NULL, (void*(*)(void*))thread_main, (void*)&tp);
    }
    return 0;
}