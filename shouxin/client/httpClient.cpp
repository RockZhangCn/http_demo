/* This is a http client demo*/

#include<iostream>
#include<fstream>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<unistd.h>
#include<sys/ioctl.h>

using namespace std;
#define bufsize 1024           //缓冲区大小

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
void saveFile();

/* 缓冲区变量  */
char recvbuf[bufsize];      //接收缓冲区
char sendbuf[bufsize];      //发送缓冲区
char content[bufsize];      //保存响应的文件内容

/* 全局变量  */
char url[100];              //用户输入的url  eg.  /index.html
char ipaddr[20];            //服务器ip地址
int port;                   //服务器端口号
char* filename;             //请求的文件名，用于保存
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
   cout<<"目的IP："<<endl;
   cin>>ipaddr;
   cout<<"目的端口："<<endl;
   cin>>port;
   string ip =ipaddr;
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
	    
	    cout<<"输入请求url:"<<endl;
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
		cout<<"收到来自"<<inet_ntoa(srv_addr.sin_addr)<<":"<<srv_addr.sin_port<<"的响应："<<endl;
        cout<<endl;
		cout<<recvbuf<<endl;

		//保存文件到本地
		savefile();

		memset(&recvbuf,0,sizeof(recvbuf));
		cout<<endl;
      }

        close(srv_sock);
}

/* 保存文件  */
void saveFile()
{
		//从url中获取文件名
		char* temp = strtok(url,"/");
		while(temp!=NULL)
		{       
				filename=temp;
				temp=strtok(NULL,"/");
		}
        char* queryTag = strtok(filename,"?");
		if(queryTag!=NULL)
				filename = queryTag;

		//写入文件
		ofstream outfile(filename);
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
                int recvlen = strlen(recvbuf);
				strncpy(content,recvbuf+4+i,recvlen-i-4);
	            outfile<<content;
				char* path = getcwd(NULL,0);
				cout<<"文件已保存到"<<path<<"/"<<filename<<endl;
		}
		outfile.close();

}
