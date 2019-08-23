/*This is a http server demo*/


#include<iostream>
#include<string.h>
#include<stdio.h>
#include<netinet/in.h>
#include<unistd.h>
#include<pthread.h>
#include<sys/ioctl.h>
using namespace std;

#define buf 1024
char sendbuf[buf];
char recvbuf[buf];
#define port 8086

void* handleRequest(void* sock);
void* recvMsg(void* sock);
void* sendMsg(void* sock);

int main()
{
   int srv_sock,clt_sock;//套接字文件描述符
   struct sockaddr_in srv_addr,clt_addr;//网络地址
   socklen_t addrlen;//bind函数指定的地址类型
   pthread_t p;//处理事务的线程
   pthread_t psend,precv;  
   /*初始化server */
   /*socket函数用于创建一个socket文件描述符
	*params：协议族：决定socket的地址类型
	*        socket类型：TCP一般是流式套接字
	*        传输协议：通常设置为0，由type指定，因为一个协议和类型是对应的
	* */       
   srv_sock = socket(AF_INET,SOCK_STREAM ,IPPROTO_TCP);
   
   //初始化结构变量，并赋值
   memset(&srv_addr,0,sizeof(srv_addr));
   srv_addr.sin_family = AF_INET;
   srv_addr.sin_port =htons(port);
   srv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
   //绑定套接字
   if(bind(srv_sock,(struct sockaddr*)&srv_addr,sizeof(srv_addr))==-1)
		   cout<<"绑定失败！！"<<endl;
   //设置监听事件
   if(listen(srv_sock,5)==-1)
		   cout<<"监听失败"<<endl;
   cout<<"等待客户端连接..."<<endl;
   //循环等待连接请求
   ioctl(srv_sock,FIONBIO,1);
    while(1)
	{
			addrlen = sizeof(clt_addr);
			clt_sock = accept(srv_sock,(struct sockaddr*)&clt_addr,&addrlen);
			if(clt_sock == -1)
					cout<<"连接失败"<<endl;
	       
			
			cout<<"welcome"<<endl;
			//多线程处理连接
		//	pthread_create(&p,NULL,handleRequest,&clt_sock);
		//	pthread_detach(p);
           pthread_create(&psend,NULL,sendMsg,(void*)&clt_sock);
        	pthread_create(&precv,NULL,recvMsg,(void*)&clt_sock);
		pthread_detach(psend);
      	pthread_detach(precv);

			memset(&clt_addr,0,sizeof(clt_addr));
	}   
    close(srv_sock);
    return 0;
}


/*独立线程发送数据，避免挂起等待*/
void* sendMsg(void* sock)
{
   int fd = (*(int*)sock);
   
   while(true)
   {   
	
       cin>>sendbuf;

       if(send(fd,sendbuf,sizeof(sendbuf),0)==-1)
               cout<<"send error"<<endl;
           
       memset(&sendbuf,0,sizeof(sendbuf));
  
   }   
   
   return 0;

}

/*独立线程接收数据，避免挂起等待*/
void* recvMsg(void* sock)
{
   int fd = (*(int*)sock);
   
   while(true)
   {   
 
       if(recv(fd,recvbuf,sizeof(recvbuf),0)==-1)
               cout<<"recv error"<<endl;
        else
		{
       cout<<recvbuf<<"        recv"<<endl;                                                                          
       memset(&recvbuf,0,sizeof(recvbuf));
  

		}
   }   
   
   return 0;

}

/*处理http请求*/
void* handleRequest(void* sock)
{
	int fd = (*(int*) sock);
	char sendbuf[buf]= "hello client";
	while(1)
	{
    if(recv(fd,recvbuf,sizeof(recvbuf),0)==-1)
				cout<<"recv error"<<endl;
		else
	  cout<<recvbuf<<endl;

	  memset(&recvbuf,0,sizeof(recvbuf));
       cin>>sendbuf;

	   if( send(fd,sendbuf,sizeof(sendbuf),0) == -1)
			   cout<<"send error"<<endl;

       memset(&sendbuf,0,sizeof(sendbuf));
	
	}
}



















