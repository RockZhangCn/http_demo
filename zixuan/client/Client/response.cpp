#include "response.h"

Response::Response()
{

    string Cookie = "";
    string ProtocolVersion = "";
    string Code = "";
    string Status = "";
    string Server = "";
    string TransferEncoding= "";
    string ConnectType= "";
    string Connection= "";
    string Date= "";
    int ContentLength = 0;

}
string Response::AnalysisResponseBody()
{

    auto strHeader = Source.substr(0, Source.find("\r\n\r\n"));
    auto data = Split(strHeader,"\r\n",0,true);
    if (data.size() == 0)
    {
        return ResponseBody;
    }
    auto FirstLine = Split(data[0]," ",0,true);
    if (FirstLine.size() != 3)
    {
        std::cout<<"response报文首行格式错误"<<endl;
    }
    ProtocolVersion = Trim(FirstLine[0]);
    Code = Trim(FirstLine[1]);
    Status = Trim(FirstLine[2]);
    data.erase(data.begin());
    for (auto x : data)
    {
        auto pair =Split( x,":", 1,true);
        if (pair.size() == 2)
        {
            Header[Trim(pair[0])] = Trim(pair[1]);
        }
    }
    string contentLen = Header["Content-Length"];
    ContentLength = contentLen != "" ? stoi(contentLen) : 0;
    Cookie = Header["Set-Cookie"];
    Server = Header["Server"];
    TransferEncoding = Header["Transfer-Encoding"];
    ConnectType = Header["ConnectType"];
    Connection = Header["Connection"];
    Date = Header["Date"];
    auto bodyPos = Source.find("\r\n\r\n");
    if (bodyPos != -1 && bodyPos != Source.length() - 4)
    {
        ResponseBody = Source.substr(bodyPos + 4);

    }
    return ResponseBody;
}
Response::~Response()
{

}
