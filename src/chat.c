#include "chat.h"

void handleErr(int fd,const char const * msg)
{
    printf("%s\n",msg); //show error message
    close(fd);  //close open socket
}

int recv_msg(int fd, char* buf,int maxlen)
{
    recv(fd,buf,maxlen,0);
    fputs(buf,stdout);  //show received message
    int len = strlen(buf);
    buf[len-1] = 0; //delete newline character
    --len;
    len = len>4?len:4;  //in order to detect string exactly the same as QUIT
    return strncmp(buf,"QUIT",len);
}

int send_msg(int fd, char* buf, int maxlen)
{
    fgets(buf,maxlen,stdin);    //receive message to transmit via terminal
    send(fd,buf,maxlen,0);
    int len = strlen(buf);
    buf[len-1] = 0; //delete newline character
    --len;
    len = len>4?len:4;  //in order to detect string exactly the same as QUIT
    return strncmp(buf,"QUIT",len);
}