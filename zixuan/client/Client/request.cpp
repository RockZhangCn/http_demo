#include "request.h"
Request::Request()
{
    m_strSource = "";
    m_strMethod = "";
    m_strPath = "";
    m_strProtocolVersion = "";
    m_strIP = "";
    m_strHost = "";
    m_strCookie = "";
    m_strRequestBody = "";
    m_strProtocolVersion = "HTTP/1.1";
}
int Request::connect_TimeOut(int socket, sockaddr_in saddr)
{
    if(socket < 0)
        return -1;
    int error=-1, len;
    len = sizeof(int);
    timeval tm;
    fd_set set;
    unsigned long ul = 1;
    ioctl(socket, FIONBIO, &ul); //设置为非阻塞模式
    bool ret = false;
    if( connect(socket, (struct sockaddr *)&saddr, sizeof(saddr)) == -1)
    {
        tm.tv_sec =TIME_OUT_TIME;
        tm.tv_usec = 0;
        FD_ZERO(&set);
        FD_SET(socket, &set);
        if( select(socket+1, NULL, &set, NULL, &tm) > 0)
        {
            getsockopt(socket, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            if(error == 0)
                ret = true;
            else
                ret = false;
        }
        else
            ret = false;
    }
    else
        ret = true;
    ul = 0;
    ioctl(socket, FIONBIO, &ul); //设置为阻塞模式
    if(!ret)
    {
        close( socket );
        std::cout<<"Cannot Connect the server!"<<endl;
        return-1;
    }
    return 0;
}
int Request::Connection(int socket, string host)
{

    int port = 80;
    auto indexPort = host.find(":");
    if (indexPort != -1)
    {
        port = stoi(host.substr(indexPort + 1));
        m_strHost=host = host.substr(0, indexPort);//去掉端口
    }

    if(!haveLetter(host))
    {
        port = 6888;
        m_strIP = host;

    }
    else {
        m_strIP =  getIpByHost(host);

    }
    if(strncmp(  "error",m_strIP.c_str(), 5) == 0)
    {
        return -1 ;
    }
    in_addr sa;
    inet_pton(AF_INET, m_strIP.c_str(), &sa) ;
    sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    saddr.sin_addr = sa;
    if(connect_TimeOut(socket,saddr)<0)
    {
        return -1;
    }
    return 0;
}
void Request::BuildRequestString()
{
    Header["Cookie"] = m_strCookie;
    Header["Host"] = m_strHost;
    if (m_strRequestBody != "")
    {
        Header["Content-Length"] = std::to_string(m_strRequestBody.length());
    }
    m_strSource = m_strMethod + " " + m_strPath + " " + m_strProtocolVersion + "\r\n";
    for (auto x : Header)
    {
        m_strSource += x.first + ":" + x.second + "\r\n";
    }
    m_strSource += "\r\n";
    m_strSource += m_strRequestBody;
}
string Request::getResponseBody(int socket)
{
    char responseBuf[MAXLINE] = { 0 };
    recv(socket, responseBuf, sizeof(responseBuf) - 1, 0);
    std::cout<<"RESPONSE:"<<endl;
    std::cout<<responseBuf<<endl;
    Response response;
    response.Source  = + responseBuf;
    response.AnalysisResponseBody();
    close(socket);
    return response.ResponseBody;
}
void Request::setMethod(MethodEnum method)
{

    switch (method)
    {
    case MethodEnum::Get:
        m_strMethod = "GET";
        break;
    case MethodEnum::Post:
        m_strMethod = "POST";
        break;
    case MethodEnum::Put:
        m_strMethod = "PUT";
        break;
    case MethodEnum::Delete:
        m_strMethod = "DELETE";
        break;
    default:
        break;
    }
}

string Request::sendRequest(string url)
{
    string protocolStr = "http://";
    url.erase(0, protocolStr.length());
    m_strHost = url.substr(0, url.find("/"));
    url.erase(0, m_strHost.length());
    m_strPath = url.length() != 0 ? url : "/";
    auto socket = getSocket();
    if(Connection(socket, m_strHost)==-1)
    {
        std::cout<<"你的输入有误请从新输入"<<endl;
        return "error";
    }
    BuildRequestString();
    std::cout<<"REQUEST:"<<endl;
    std::cout<<m_strSource<<endl;
    send(socket, m_strSource.c_str(), int(m_strSource.length()), 0);
    return getResponseBody(socket);
}

void Request::sendRequest(MethodEnum method, string url)
{
    setMethod(method);
    string strResponseBody = sendRequest(url);
    if(strncmp(  "error",strResponseBody.c_str(), 5) != 0)
    { std::cout<<"\r\n\r\n解析的ResponseBody:"<<endl;
        std::cout<<strResponseBody<<endl;
    }

}

int Request::getSocket()
{
    return socket(AF_INET, SOCK_STREAM, 0);
}
string Request::getIpByHost(string hostname)
{
    addrinfo hints, *result;
    in_addr addr;
    memset(&hints, 0, sizeof(addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;//ipv4
    if ((getaddrinfo(hostname.c_str(), NULL, &hints, &result)) != 0) {
        std::cout<<"DNS ERROR:"<<endl;
        return "error";
    }

    addr.s_addr = ((sockaddr_in*)(result->ai_addr))->sin_addr.s_addr;
    char str[INET_ADDRSTRLEN];
    auto ptr = inet_ntop(AF_INET, &addr, str, sizeof(str));
    freeaddrinfo(result);
    return str;

}
bool Request::haveLetter(string  str)
{
    for (auto x : str)
    {
        if ((x >= 'a'&&x <= 'z') ||
                (x >= 'A'&&x <= 'Z'))
        {
            return true;
        }
    }
    return false;
}
Request::~Request()
{

}
