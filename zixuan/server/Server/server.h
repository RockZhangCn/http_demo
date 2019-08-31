#ifndef CLIENT_H
#define CLIENT_H

#include<stdio.h>
#include <iostream>
#include  <unistd.h>
#include  <sys/types.h>       /* basic system data types */
#include  <sys/socket.h>      /* basic socket definitions */
#include  <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include  <arpa/inet.h>       /* inet(3) functions */

#include <stdlib.h>
#include <errno.h>
#include "stdio.h"
#include "server.h"
#include "string.h"
#include "time.h"
using namespace std;
#define MAXLINE 1024
#define LISTENMAX 1024
class Server
{
public:
    Server();
    ~Server();
    int Listen();//启动监听
    int Start();//开始服务
private:
    void Init();//初始化服务端配置
    void handle(int connfd);//自定义处理流程

private:
    int  m_iListenfd;//监听Socket端口
    int m_iConnectfd;//连接端口
    int  m_iServerPort ;//服务端端口
    pid_t   m_ChildpID;
    char buf[MAXLINE];
    socklen_t socklen;

    struct sockaddr_in m_Clientddr;//客户端信息
    struct sockaddr_in m_Servaddr;//服务端信息
};

#endif // CLIENT_H
