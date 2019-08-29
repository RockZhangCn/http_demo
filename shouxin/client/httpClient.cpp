/* This is a http client demo*/

#include<iostream>
#include<fstream>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
#include<stdio.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<netdb.h>
#include<unistd.h>
#include<sys/ioctl.h>
#include<sys/stat.h>
#include<sys/types.h>
using namespace std;
#define bufsize 20000           //缓冲区大小

/* 请求首部信息  */
const string method = "GET ";
const string version = " HTTP/1.1 \r\n";
const string connection = "Connection: keep-alive\r\n";
const string user_agent = "User-Agent: vivo-nanjing-11103875\r\n";
const string acpt = "Accept: */* \r\n";
const string encoding = "Accept-Endcoding: gzip, deflate\r\n";
const string language = "Accept-Language: zh-CN,zh;q=0.9\r\n";
const string space = "\r\n";
string host;                   


/* 方法声明  */
void requestData();
void connectToServer();
void init();
string  getIP(string server);
void saveFile();
void mkdirs(char* path);

/* 缓冲区变量  */
char recvbuf[bufsize];      //接收缓冲区
char sendbuf[bufsize];      //发送缓冲区
char content[bufsize];      //保存响应的文件内容

/* 全局变量  */
char url[50];              //用户输入的url  eg.  /index.html
char ipaddr[20];            //服务器ip地址
int port;                   //服务器端口号
int srv_sock;               //套接字文件描述符
struct sockaddr_in srv_addr;//网络地址


/* 主程序  */
int main()
{

   //初始化套接字和地址变量
   init();
   //向服务器发起连接请求
   connectToServer();
   //向服务器请求数据并保存
   requestData();
   
   return 0;
}


/* 初始化全局变量  */
void init()
{
   //获取服务器信息
   cout<<"目的地址："<<endl;
   cin>>ipaddr;
   cout<<"目的端口："<<endl;
   cin>>port;
   string ip =getIP(ipaddr);
   strcpy(ipaddr,ip.data());
   host = "Host: "+ip+":"+to_string(port)+"\r\n";
   //创建socket
   srv_sock = socket(AF_INET,SOCK_STREAM,0);
   if(srv_sock==-1)
   {
	  cout<<"创建socket失败"<<endl;
	  exit(1);
   }
   //初始化结构变量，并赋值
   memset(&srv_addr,0,sizeof(srv_addr));
   srv_addr.sin_family = AF_INET;
   srv_addr.sin_port =htons(port);
   srv_addr.sin_addr.s_addr = inet_addr(ipaddr);
}

string getIP(string server)
{
		char first=server.at(0);
		
		if(first>='0'&&first<='9')
				return server;

		char ipstr[17];
		string ipaddr = "";
		struct addrinfo hints;

		memset(&hints,0,sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
        
		struct addrinfo * value;
		if(getaddrinfo(server.data(),NULL,&hints,&value)!=0)
		{
				cout<<"DNS 解析失败"<<endl;
				exit(1);
		}
		else
		{
				struct addrinfo* ptr = value;
			    
				struct sockaddr_in *s = (sockaddr_in*)ptr->ai_addr;
			
	     	    inet_ntop(AF_INET,&s->sin_addr.s_addr,ipstr,sizeof(ipstr)); 

				ipaddr = ipstr;
		}

		freeaddrinfo(value);
		return ipaddr;


}

/* 连接到服务器  */
void connectToServer()
{
  if( connect(srv_sock,(struct sockaddr*)& srv_addr,sizeof(srv_addr))==-1)
  {
		  cout<<"连接失败"<<endl;
          exit(1);
  }
  else
  cout<<"成功连接到"<<inet_ntoa(srv_addr.sin_addr)<<" "<<ntohs(srv_addr.sin_port)<<endl;
  
}

/*  发送http请求  */
void requestData()
{
	  while(1)
	  {
	    
	    cout<<"输入请求url:  (输入exit退出连接)"<<endl;
	    cin>>url;
	    
	    cout<<endl;
		
        //退出处理
	    if(strcmp(url,"exit")==0)
	    { 
          send(srv_sock,url,strlen(url),0 );
		  cout<<"已断开连接"<<endl;
		  break;
	    }    
       //组装请求报文
	   string request = method+url+version+host+connection+user_agent+acpt+encoding+language+space+"\0";
	   strcpy(sendbuf,request.data());
       //发送请求
	   send(srv_sock,sendbuf,strlen(sendbuf),0);
       memset(&sendbuf,0,sizeof(sendbuf));
	    /*等待接收响应*/
	   if(recv(srv_sock,recvbuf,sizeof(recvbuf),0)==-1)
    	{
			cout<<"接受文件错误！！"<<endl;
			exit(1);
     	}
		cout<<"收到来自"<<inet_ntoa(srv_addr.sin_addr)<<":"<<ntohs(srv_addr.sin_port)<<"的响应："<<endl;
        cout<<endl;
		cout<<recvbuf<<endl;

		//保存文件到本地
		saveFile();
        memset(&url,0,sizeof(url));
		memset(&recvbuf,0,sizeof(recvbuf));
		cout<<endl;
      }

        close(srv_sock);
}

/* 保存文件  */
void saveFile()
{      
		
		char tok[50];
		strcpy(tok,url);            

        char* filename;             //请求的文件名，用于保存
        char filepath[100];         //保存url路径，便于保存文件
        char resultCode[10];        //判断响应码

		//只有正确响应才保存文件
		strncpy(resultCode,recvbuf+9,3);
	    if(strcmp(resultCode,"200")!=0)
				return;

        //从URL分割出文件名
		char* temp = strtok(tok,"/");
		while(temp!=NULL)
		{
				filename = temp;
				temp = strtok(NULL,"/");
		}
        //把查询字段剔除掉
        char* queryTag = strtok(filename,"?");
		if(queryTag!=NULL)
				filename = queryTag;
	    
        //从url获取文件路径
		int nameLen = strlen(filename);
		int urlLen = strlen(url);

		strncpy(filepath,url,urlLen-nameLen);

        //绝对路径拼接
		string sfilePath = filepath;
		string current_path = getcwd(NULL,0);
        string full_path = current_path+sfilePath;
   
        strcpy(filepath,full_path.c_str());
        //本地新建目录保存
        mkdirs(filepath);

		//写入文件
		ofstream outfile(full_path+filename);
		if(!outfile.is_open())
		{
				cout<<"文件写入失败"<<endl;
		}
		else
		{   
				//获取文件信息
				int i = 0;
				while(recvbuf[i]!='\0')
				{
						if(recvbuf[i]=='\r'&&recvbuf[i+2]=='\r')
								break;
						i++;
				}
				//提取报文主体
                int recvlen = strlen(recvbuf);
				strncpy(content,recvbuf+4+i,recvlen-i-4);
	            outfile<<content;
				cout<<"文件已保存到"<<full_path<<filename<<endl;
		}
	
		memset(&content,0,sizeof(content));
		outfile.close();

}

/* 目录新建  */
void mkdirs(char *path)
{

		char str[100];
		strcpy(str,path);

		int len = strlen(str);

		for(int i=0;i<len;i++)
		{
				if(str[i]=='/')
				{
						str[i]='\0';
						if(access(str,0)!=0)
						{
								mkdir(str,0777);
						}
						str[i]='/';
				}
		}
		if(len>0&&access(str,0)!=0)
         mkdir(str,0777);
}
