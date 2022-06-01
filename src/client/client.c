#include "client.h"

void sig_handle_c(int signo)
{
    if(signo == SIGINT) //handle SIGINT
    {
        close(cs);  //close open socket
        puts("");   //to maintain terminal allignment
        exit(1);    //terminate process
    }
}

void usage()
{
    printf("./chat-client [SERVER-IP] [SERVER-PORT] [NICKNAME]\n");    //show usage
    puts("NICKNAME LEN <= 20bytes");
}

int init_socket(struct sockaddr_in *sockaddr,char* ip, char* port)
{
    char* pEnd = 0;
    errno = 0;
    uint16_t port_num = strtoul(port,&pEnd,10); //atoi can't detect error
    if(port == pEnd || errno == ERANGE || *pEnd!=0) return -1;  //strtoul failure

    in_addr_t IP = inet_addr(ip);   //transform IP from dot-separated format to 32bit integer
    if(IP == INADDR_NONE) return -1;    //transformation failure

    sockaddr->sin_family = AF_INET; //IPv4
    sockaddr->sin_port = htons(port_num);   //transform port from host order to network order
    (sockaddr->sin_addr).s_addr = IP;

    return 0;
}

void conn_succ_client()
{
    printf("Connected\n");  //client connection success message
}


void recv_thread(void* param)
{
    char recv_buf[MAX_BUF_LEN] = {0};   //buffer used to receive message
    while(1)
    {
        recv(cs,recv_buf,sizeof(recv_buf),0);
        puts(recv_buf);
        memset(recv_buf,0,MAX_BUF_LEN);
        
        //condition variable???
    }
}

void chat_client(char* nickname)
{
    uint8_t flag = 1;
    char send_buf[MAX_BUF_LEN] = {0};   //buffer used to send message

    pthread_t tid;
    pthread_create(&tid, NULL, (void*(*)(void*))recv_thread, NULL);

    //send nickname
    int len = strlen(nickname);
    send(cs,nickname,len,0);

    //send msg
    while(1)
    {
        flag = send_msg(cs,send_buf,sizeof(send_buf));
        if(flag == 0)
        {
            printf("Disconnected\n");   //flag==0 means QUIT has transmitted
            break;
        }
        memset(send_buf,0,MAX_BUF_LEN);
    }

}