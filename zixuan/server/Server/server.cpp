#include "server.h"
Server::Server()
{
    Init();
}
void Server::Init()
{
    m_iServerPort = 6888;
    socklen = sizeof(m_Clientddr);
    bzero(&m_Servaddr, sizeof(m_Servaddr));
    m_Servaddr.sin_family = AF_INET;
    m_Servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    m_Servaddr.sin_port = htons(m_iServerPort);

}
int Server::Listen()
{
    m_iListenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_iListenfd < 0) {
        perror("socket error");
        return -1;
    }
    if (bind(m_iListenfd, (struct sockaddr *) &m_Servaddr, socklen) < 0) {
        perror("bind error");
        return -1;
    }
    if (listen(m_iListenfd, LISTENMAX) < 0) {
        perror("listen error");
        return -1;
    }
    std::cout<<" server startup,listen on port:"<<m_iServerPort<<"\n";
    return 0;

}
int Server::Start()
{
    for ( ; ; )  {
        m_iConnectfd = accept(m_iListenfd, (struct sockaddr *)&m_Clientddr, &socklen);
        if (m_iConnectfd < 0) {
            perror("accept error");
            return -1 ;
        }
        std::cout<<"accept form IP:"<<inet_ntoa(m_Clientddr.sin_addr)<<"port:"<<m_Clientddr.sin_port<<"\n";
        m_ChildpID = fork();
        if (m_ChildpID == 0) {
            close(m_iListenfd);
            handle(m_iConnectfd);
            exit (0);
        } else if (m_ChildpID > 0)  {
            close(m_iConnectfd);
        } else {
            perror("fork error");
        }
    }
}

Server::~Server()
{

}

void Server::handle(int connfd)
{
    size_t n;
    char  buf[MAXLINE];

    for(;;) {
        n = recv(connfd, buf, MAXLINE,0);
        if (n < 0) {
            if(errno != EINTR) {
                perror("read error");
                break;
            }
        }
        if (n == 0) {
            close(connfd);
            std::cout<<"client exit\n";
            break;
        }
        if (strncmp("exit", buf, 4) == 0) {
            close(connfd);
            printf("client exit\n");
            break;
        }
        string str = buf;
        if(str.find_first_of("GET")==0)
        {
            char Headerbuf[MAXLINE] ="HTTP/1.1 200 OK \r\n\r\n\r\n";
            send(connfd, Headerbuf, sizeof(Headerbuf), 0);

        }

    }

}


