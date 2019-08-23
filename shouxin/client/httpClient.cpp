/* This is a http client demo*/

#include<iostream>
#include<sys/types.h>
#include<arpa/inet.h>
#include<string.h>
#include<sys/socket.h>
#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>

using namespace std;
#define buf 1024
char recvbuf[buf];
char sendbuf[buf];
char ipaddr[]="172.22.211.202";
#define port 8086
void* sendMsg(void* sock);
void* recvMsg(void* sock);
void* httpRequest(void* sock);

int main()
{
   int srv_sock,clt_sock;//套接字文件描述符
   struct sockaddr_in srv_addr,clt_addr;//网络地址
   socklen_t addrlen;//bind函数指定的地址类型
   pthread_t precv,psend;//处理事务的线程
   
   srv_sock = socket(AF_INET,SOCK_STREAM,0);
   
   //初始化结构变量，并赋值
   memset(&srv_addr,0,sizeof(srv_addr));
   srv_addr.sin_family = AF_INET;
   srv_addr.sin_port =htons(port);
   srv_addr.sin_addr.s_addr = inet_addr(ipaddr);
   
  if( connect(srv_sock,(struct sockaddr*)& srv_addr,sizeof(srv_addr))==-1)
     cout<<"连接失败"<<endl;
    cout<<"成功连接到"<<inet_ntoa(srv_addr.sin_addr)<<" "<<ntohs(srv_addr.sin_port)<<endl;
    while(1)
	{
			/*准备发送数据*/
			/*请输入发送的数据：*/
		//	cin>>sendbuf;

		//	send(srv_sock,sendbuf,sizeof(sendbuf),0);
            /*等待接收数据...*/
		//	recv(srv_sock,recvbuf,sizeof(recvbuf),0);
            
		//	cout<<recvbuf<<endl;
			pthread_create(&psend,NULL,sendMsg,(void* )&clt_sock);
			pthread_create(&precv,NULL,recvMsg,(void*)&clt_sock);
			pthread_detach(psend);
			pthread_detach(precv);


			
	}   
    close(srv_sock);
    return 0;
}


void* sendMsg(void* sock)
{
   int fd = (*(int*)sock);
   
   while(1)
   {
       cin>>sendbuf;
	   if(send(fd,sendbuf,sizeof(sendbuf),0)==-1)
			   cout<<"send error"<<endl;
       
	   memset(&sendbuf,0,sizeof(sendbuf));
	   cin.clear();
       cin.sync();
   }
   
   return 0;

}

void* recvMsg(void* sock)
{
   int fd = (*(int*)sock);
   
   while(1)
   {
 
	   if(recv(fd,recvbuf,sizeof(sendbuf),0)==-1)
			   cout<<"send error"<<endl;
       
	   cout<<recvMsg<<"recv"<<endl;
	   memset(&recvbuf,0,sizeof(sendbuf));
  
   }
   
   return 0;

}

/*  发送http请求 
 *  arg:与服务器绑定的套接字
 *  */
void* httpRequest(void* sock)
{

   int fd = (*(int*)sock);
   

   while(1)
   {
 
	   if(recv(fd,recvbuf,sizeof(sendbuf),0)==-1)
			   cout<<"recv error"<<endl;
       
	   cout<<recvMsg<<endl;
	   memset(&recvbuf,0,sizeof(sendbuf));
  
   }
   
   return 0;

}



