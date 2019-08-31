
#include"request.h"
void Clientstart()
{
    for(;;)
    {
        std::cout <<"请输入你要访问的网站,exit退出程序" <<std::endl;
        std::cout <<"--------------------------------------------------------------------------------" <<std::endl;
        char inputLine[MAXLINE];
        if (fgets(inputLine, MAXLINE, stdin) == NULL) {
            continue;//read eof
        }
        if (strncmp("exit", inputLine, 4) == 0)
        {
            break;
        }
        string strHost = inputLine;
        strHost = "http://"+ strHost;//网址自动补全
        strHost = strHost.substr(0,strHost.length()-1);
        Request request;
        request.sendRequest(MethodEnum::Get, strHost);
        std::cout <<"--------------------------------------------------------------------------------" <<std::endl;
    }



}
int main()
{

    Clientstart();

}
