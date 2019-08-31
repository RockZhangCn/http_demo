#ifndef RESPONSE_H
#define RESPONSE_H
#include"common.h"
using namespace  std;

class Response
{

public:
    ~Response();
    Response() ;
    string AnalysisResponseBody();//解析response
    string Source;//报文字符串
    string ResponseBody;
private:

    string Cookie;//Cookie信息
    string ProtocolVersion ;//协议版本
    string Code ;//状态码2XX,3XX,4XX,5XX
    string Status ;//原因短语
    string Server ;//服务器信息
    string TransferEncoding;//实体编码格式
    string ConnectType;//实体媒体类型 image/gif
    string Connection;//连接管理 Keep-Live Close
    string Date;//报文创建时间
    int ContentLength ;//body长度
    std::map<string, string> Header;//报文首部

};

#endif // RESPONSE_H
