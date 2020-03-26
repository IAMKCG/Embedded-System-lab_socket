// server 服务器端
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

#define MyPort 3333

int main()
{
    int sockfd, fd;
    struct sockaddr_in server_addr,  client_addr;
    socklen_t len;
    fd_set rfds;
    char _content[1024];

    //服务器自身建立一个socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    //给套接字指定端口和IP
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MyPort);
    server_addr.sin_addr.s_addr = htons(INADDR_ANY);

    //将套接字和端口及IP绑定
    bind(sockfd, (struct sockaddr *) &server_addr, sizeof(struct sockaddr));

    //开始监听
    if (listen(sockfd, 5) == -1) {printf("Listen Error.\n");exit(0);}
    else {printf("Listening.\n");}
    //开始工作
    while(1)
    {
        len = sizeof(struct sockaddr);

        //建立连接
        fd = accept(sockfd, (struct sockaddr *) &client_addr, &len);
        if (fd == -1) {printf("Link Error.\n");exit(0);}
        else printf("Link Success.\n");

        while(1)
        {
            int maxfd, res;
            FD_ZERO(&rfds);
            FD_SET(0, &rfds);
            FD_SET(fd, &rfds);
            if (fd > 0) maxfd = fd;
            else maxfd = 0;
            res = select(maxfd+1, &rfds, NULL, NULL, NULL);
            if (res == 0)
            {
                printf("Waiting...\n");
            }
            else if (res == -1) exit(0);
            else
            {
                if (FD_ISSET(0, &rfds))
                {
                    memset(_content, 0, sizeof(_content));
                    fgets(_content, 1024, stdin);

                    len = send(fd, _content, strlen(_content), 0);
                    if (len > 0) printf("Sent.\n");
                    else {printf("Failed to send.\n"); break;}
                }
                if (FD_ISSET(fd, &rfds))
                {
                    memset(_content, 0, sizeof(_content));
                    len = recv(fd, _content, 1024, 0);
                    if (len > 0) printf("Said: %s\n",_content);
                    else
                    {
                        if (len == 0) printf("Disconnected.\n");
                        if (len < 0) printf("Error.\n");
                        break;
                    }
                }
            }
        }
        close(fd);
    }
    close(sockfd);
    return 0;
}
