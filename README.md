# Embedded-System-lab_socket
Embedded System course exercises of EECS , PKU - part I : socket

嵌入式系统课程的项目练习。此处为第一部分：利用socket实现局域网的一个简单的聊天软件。

程序分为两部分。其一是server，服务器端。另一个是client，客户端。

首先是server。对于服务器而言，需要的操作应为：给服务器（主机）创建一个套接字，并指定IP和端口，将之绑定。然后开始监听。客户端发来连接请求后，和客户端建立连接。然后就可以收发消息了。具体收发利用select实现。

然后是client。对于客户端而言，需要的操作应为：给客户端创建一个套接字，并指定IP和端口（这由用户自行给出，意为给定需要连接到的服务器的信息）。然后与该服务器建立连接，建立完成后就可以收发消息了。具体收发利用select实现。
