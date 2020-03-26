// client 客户端
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MyPort 3333

//客户端运行时指定IP地址
int main(int argc, char **argv)
{
    int sockfd;
    struct sockaddr_in s_addr;
    fd_set fd;
    int res, maxfd;
    char _content[1024];
    socklen_t len;

    //客户端自身建立一个socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    //给套接字指定端口和IP
    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_port = htons(MyPort);
    if (inet_aton(argv[1], (struct in_addr *) & s_addr.sin_addr.s_addr) == 0) {printf("Address Error.\n");exit(0);}

    //与服务器连接
    if (connect(sockfd, (struct sockaddr *) & s_addr, sizeof(struct sockaddr)) == -1) {printf("Connect Error.\n");exit(0);}

    //开始工作
    while(1)
    {
        FD_ZERO(&fd); // 先清零
        FD_SET(0, &fd); // 表示客户端往服务器发消息
        FD_SET(sockfd, &fd); // 表示服务器往客户端发消息
        if (sockfd > 0) maxfd =sockfd;
        else maxfd = 0;

        // 看看有什么事件触发了
        res = select(maxfd+1, &fd, NULL, NULL, NULL);

        if (res == 0)
        {
            // 没任何事发生
            printf("Waiting...\n");
            continue;
        }
        else if (res == -1)
        {
            //出错了
            exit(0);
        }
        else
        {
            if (FD_ISSET(sockfd, &fd)) // 是服务器发来了消息
            {
                // 初始化，做好接受准备
                memset(_content, 0, sizeof(_content));

                len = recv(sockfd, _content, 1024, 0);

                if (len > 0)
                    printf("Said: %s \n", _content);
                else 
                {
                    if (len < 0)
                        printf("Error\n");
                    else if (len == 0)
                        printf("Disconnected\n");
                    
                    break;
                }
            }
            if (FD_ISSET(0, &fd)) // 是客户端要发消息
            {
                memset(_content, 0 ,sizeof(_content));
                fgets(_content, 1024, stdin);
                len = send(sockfd, _content, strlen(_content), 0);

                if (len > 0) printf("Sent.\n");
                else {printf("Failed to send.\n"); break;}
            }
        }
    }
    close(sockfd);
    return 0;
}
