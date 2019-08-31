#ifndef REQUEST_H
#define REQUEST_H
#include "common.h"
#include "response.h"
#include "sys/ioctl.h"
#include "sys/time.h"
#define TIME_OUT_TIME 10
using namespace std ;

class Request
{
public:
    Request();
    ~Request();
    string m_strSource;//报文字符串
    string m_strMethod;//方法 Get,Post,put,Delete
    string m_strPath;//文件路径
    string m_strProtocolVersion;//协议版本HTTP1.1
    string m_strIP;//IP地址
    string m_strHost;//域名
    string m_strCookie;//身份信息
    string m_strRequestBody;//发送报文Body
    std::map<string, string> Header;//报文首部
public:
    void sendRequest(MethodEnum method, string url);//对外提供的发送接口

private:

    int Connection(int socket, string host);//创建TCP链接,三次握手过程
    void BuildRequestString();//创建发送报文
    string getResponseBody(int socket);//解析Response,获取ResponseBody
    void setMethod(MethodEnum method);//设置方法
    string sendRequest(string url);
    int getSocket();
    string getIpByHost(string hostname);
    bool haveLetter(string  str);//判断字符串是否包含字母
    int connect_TimeOut(int socket,  sockaddr_in saddr);//connect可设置超时时间

};
#endif // REQUEST_H
