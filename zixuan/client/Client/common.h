#ifndef COMMON_H
#define COMMON_H
#include "string.h"
#include "sys/socket.h"
#include"sys/socket.h"
#include"arpa/inet.h"
#include "netinet/in.h"
#include "map"
#include "netdb.h"
#include "memory.h"
#include "unistd.h"
#include "iostream"
#include "vector"
#define MAXLINE 1024
using namespace std;
enum MethodEnum
{
    Get,
    Post,
    Put,
    Delete
};
string Trim(string str);//去除字符串两端的空格
std::vector<int> FindAllPosition(string str ,string flag,int num);//查询字符串内指定flag的所有位置
std::vector<string> Split(string str,string flag,int num,int skipEmpty );//按flag分割字符串

#endif // COMMON_H
