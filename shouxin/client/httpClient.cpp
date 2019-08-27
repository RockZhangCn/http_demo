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
#include<sys/ioctl.h>

using namespace std;
#define buf 1024
#define port 8086

const string method = "GET /";
const string version = " HTTP/1.1 \r\n";
const string host = "Host: 172.22.211.202:8086\r\n";
const string connection = "Connection: keep-alive\r\n";
const string user_agent = "User-Agent: vivo-nanjing-11103875\r\n";
const string acpt = "Accept: */* \r\n";
const string encoding = "Accept-Endcoding: gzip, deflate\r\n";
const string language = "Accept-Language: zh-CN,zh;q=0.9\r\n";
const string space = "\r\n";

void handleMsg();
void httpResponse();
void connectToServer();
void init();

char recvbuf[buf];
char sendbuf[buf];
char filename[20];
char ipaddr[]="172.22.211.202";
int srv_sock,clt_sock;//套接字文件描述符
struct sockaddr_in srv_addr,clt_addr;//网络地址
socklen_t addrlen;//bind函数指定的地址类型
pthread_t precv,psend;//处理事务的线程


int main()
{
   init();
   connectToServer();
 
   handleMsg();
   
   return 0;
}



void init()
{
   if(srv_sock = socket(AF_INET,SOCK_STREAM,0)==-1)
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

/*  发送http请求 
 *  arg:与服务器绑定的套接字
 *  */
void handleMsg()
{
	  while(1)
	  {
	    cout<<"请输入请求的文件名"<<endl;
	    cin>>filename;
	  // char filename[] ="index.html";
	    cout<<endl;
        //组装成GET报文
	    if(strcmp(filename,"exit")==0)
	    { 
          send(srv_sock,filename,sizeof(filename),0 );
		  cout<<"connect end"<<endl;
		  break;
	    }    
       
	   string url = method+filename+version+host+connection+user_agent+acpt+encoding+language+space+"\0";
	   strcpy(sendbuf,url.data());
	   send(srv_sock,sendbuf,strlen(sendbuf),0);
       memset(&sendbuf,0,sizeof(sendbuf));
    
	    /*等待接收响应*/
	   if(recv(srv_sock,recvbuf,sizeof(recvbuf),0)==-1)
    	{
			cout<<"recv error"<<endl;
			break;
     	}
		cout<<"收到的响应："<<endl;
		cout<<recvbuf<<endl;

		//保存为本地文件
		ofstream outfile(filename);
		if(!outfile.is_open())
		{
				cout<<"文件写入失败"<<endl;
		}
		else
	      outfile<<recvbuf;
		outfile.close();
		memset(&recvbuf,0,sizeof(recvbuf));
      }

        close(srv_sock);
}
