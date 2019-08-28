/*This is a http server demo*/


#include<iostream>
#include<fstream>
#include<string.h>
#include<stdio.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ioctl.h>

using namespace std;

#define port 8086
#define buf 2048

const string headLine = "HTTP/1.1 200 OK\r\n";
const string server = "Server: vivo-jsx\r\n";
const string content_type = "Content-Type: text/html; charset=utf-8\r\n";
const string connection = "Connection: keep-alive\r\n";
const string cache_control = "Cache-Control: private\r\n";
const string spaceline = "\r\n";


int srv_sock;//套接字文件描述符
struct sockaddr_in srv_addr,clt_addr;//网络地址结构体
socklen_t addrlen;//bind函数指定的地址类型
pthread_t p;//处理事务的线程号

/*该结构体用于填充线程入口函数参数
 *fd:套接字描述符
 *ipaddr：本线程处理的客户端ip地址
 *sock_port:客户端端口号 
 * */
struct params
{
   int fd;
   string ipaddr;
   int sock_port;
};


void* handleRequest(void* params);//工作线程

void init();//初始化操作

void handleConnection();//等待连接并分配线程处理请求


int main()
{
    init();

	handleConnection();

    return 0;
}


void init()
{
   /*初始化server socket */
        
   srv_sock = socket(AF_INET,SOCK_STREAM ,IPPROTO_TCP);
   if(srv_sock==-1)
    {
			cout<<"创建socket失败"<<endl;
			exit(1);
	}
   
   //初始化结构变量，并赋值
   memset(&srv_addr,0,sizeof(srv_addr));
   srv_addr.sin_family = AF_INET;
   srv_addr.sin_port =htons(port);
   srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

   //绑定套接字
   if(bind(srv_sock,(struct sockaddr*)&srv_addr,sizeof(srv_addr))==-1)
    {
			cout<<"绑定失败！！"<<endl;
			close(srv_sock);
			exit(1);
	}	  
   //设置监听事件
   if(listen(srv_sock,5)==-1)
   {
		   cout<<"监听失败"<<endl;
	       close(srv_sock);
		   exit(1);
   }
		  

}

void handleConnection()
{
   cout<<"等待客户端连接..."<<endl;
   //循环等待连接请求
    
    while(1)
	{
			addrlen = sizeof(clt_addr);
			//接收连接请求
		    int	clt_sock = accept(srv_sock,(struct sockaddr*)&clt_addr,&addrlen);
			if(clt_sock == -1)
			{
                   cout<<"连接失败"<<endl;
				   exit(1);
			}	
			string ipaddr = inet_ntoa(clt_addr.sin_addr);
            int sock_port = ntohs(clt_addr.sin_port);
			cout<<"Connect to:"<<ipaddr<<":"<<sock_port<<endl;
			cout<<endl;
			
			//多线程处理连接
			struct params *arg;
			arg = (struct params*)malloc(sizeof(struct params));
			arg->fd = clt_sock;
			arg->ipaddr = ipaddr;
			arg->sock_port=sock_port;

			pthread_create(&p,NULL,handleRequest,(void*)arg);
			pthread_detach(p);
       
			memset(&clt_addr,0,sizeof(clt_addr));
	}   
    close(srv_sock);

}


/* 处理http请求 
 * 线程工作函数
 * */
void* handleRequest(void* para)
{
	//提取参数
	struct params* arg;
	arg = (struct params *)para;
	int fd = arg->fd;
	string ipaddr = arg->ipaddr;
	int sock_port = arg->sock_port;
	
	//buffers
    char sendbuf[buf];
	char recvbuf[buf];
    char method[20];
    char url[30];

	//发送文本
	string s_send;
	//数据文本
	string s_text;

	while(1)
	{
      //读取数据
      if(recv(fd,recvbuf,sizeof(recvbuf),0)==-1)
      {
          cout<<"recv error"<<endl;
          exit(1);
      }
	  else if(strcmp(recvbuf,"exit")==0)
	  {
			  cout<<ipaddr<<":"<<sock_port<<" disconnected"<<endl;
			  close(fd);
			  break;
	  }
      else
      {
		  
		  cout<<"接收到来自"<<ipaddr<<":"<<sock_port<<"的请求:\r\n"<<recvbuf<<endl;
		  //分解字符串,取出URL
		  if(strstr(recvbuf,"HTTP")==NULL)
		  {
				  char requestError[]="本服务器只接受HTTP GET请求";
				  cout<<"无效的请求"<<endl;
				  send(fd,requestError,sizeof(requestError),0);
				  continue;
		  }
          strcpy(method,strtok(recvbuf,"/"));
		  if(strcmp(method,"GET")==0)
		  {
                  char not_get[]="本服务器仅处理GET请求";
				  cout<<"非GET HTTP请求"<<endl;
				  send(fd,not_get,sizeof(not_get),0);
				  continue;
		  }
          strcpy(url,strtok(NULL," "));
	     
          //从文本文k件中读取数据
          ifstream fin(url);
		  if(!fin.is_open())
		  {
				  char no_file[50]="没有该文件！请确认文件名\r\n";
				  cout<<"没有该文件"<<endl;
				  send(fd,no_file,sizeof(no_file),0);
				  continue;
		  }
          string s;
		  while(getline(fin,s))
		  {
                s_text = s_text + s + "\r\n";  
		  }
		  //组装响应报文
		  int size = s_text.length();
          s_send = headLine+server+content_type+"Content-Size: "+to_string(size)+"\r\n"+connection+cache_control+spaceline+s_text+"\0";
		  
		  strcpy(sendbuf,s_send.data());

		 if(send(fd,sendbuf,strlen(sendbuf),0) == -1)
		 {
				 cout<<"response error"<<endl;
				 exit(1);
		 }
		 else
				 cout<<"response success!"<<endl;
	  
     	 //清空缓存
		 memset(&sendbuf,0,sizeof(sendbuf));
		 memset(&recvbuf,0,sizeof(recvbuf));
		 s_send="";
		 s_text="";
		 fin.close();
		 
	  }
	  
	}

}


