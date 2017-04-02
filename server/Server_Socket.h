#ifndef _SERVERSOCKET_H
#define _SERVERSOCKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include <errno.h>
#include <pthread.h>

using namespace std;

int createServSock(){
	int serv_sock=socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	return serv_sock;
}
int bindServSock(int serv_sock, string IP, int port){
	sockaddr_in serv_addr;
   	memset(&serv_addr, 0, sizeof(serv_addr));  //每个字节都用0填充
   	serv_addr.sin_family = AF_INET;  //使用IPv4地址
	char* ch=(char*)IP.c_str();
   	serv_addr.sin_addr.s_addr = inet_addr(ch);  //具体的IP地址
   	serv_addr.sin_port = htons(port);  //端口
   	return bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		cout<<"bind"<<endl;
	}
int listenServSock(int serv_sock, int num){
	return listen(serv_sock, num);
}
int createClntSock(int serv_sock){
  	struct sockaddr_in clnt_addr;
   	socklen_t clnt_addr_size = sizeof(clnt_addr);
    int clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
	return clnt_sock;
}
int my_read(int fd,char *buffer,int length){
	int bytes_left;
	int bytes_read;
	char *ptr;
	ptr=buffer;
   
	bytes_left=length;
	while(bytes_left>0){
  		bytes_read=read(fd,buffer,bytes_left);
//		cout<<"read="<<bytes_read<<endl;
  				if(bytes_read<0){
     				if(errno==EINTR)
        				bytes_read=0;
     				else
        				return(-1);
   				}
   				else if(bytes_read==0)
       				break;
    		bytes_left-=bytes_read;
//			cout<<"left="<<bytes_left<<endl;
    		ptr+=bytes_read;
			}
			return(length-bytes_left);
		}
string readInfo(int clnt_sock,int size){
	char buffer[size];
	my_read(clnt_sock, buffer, size);
	string str=buffer;
	return str;
}

int my_write(int fd,char *buffer,int length){
	int bytes_left;
	int written_bytes;
	char *ptr;
 
	ptr=buffer;
	bytes_left=length;
	while(bytes_left>0){
                /* 开始写*/
        		written_bytes=write(fd,ptr,bytes_left);
        		if(written_bytes<=0) /* 出错了*/{       
                	if(errno==EINTR) /* 中断错误 我们继续写*/
                        written_bytes=0;
                else             /* 其他错误 没有办法,只好撤退了*/
                        return(-1);
        		}
        		bytes_left-=written_bytes;
        		ptr+=written_bytes;     /* 从剩下的地方继续写  */
			}
				return(0);
		}
void writeInfo(int clnt_sock ,string str){
	char* buffer=(char *)str.c_str();
	my_write(clnt_sock, buffer, strlen(buffer));
}


void closeClnt(int clnt_sock){
		close(clnt_sock);
}
void closeServ(int serv_sock){
		close(serv_sock);
}

#endif

/*void* communcation (void* args){
		int clnt_sock=(int) (*((int *)args));
		cout<<"I got "+readInfo(clnt_sock, 10)<<endl;
		writeInfo(clnt_sock,"hahaha");
		return NULL;
}


int main(int argc, char* argv[]){
	int serv_sock;
	int clnt_sock;
	pthread_t pth[3];
	int i=0;
	serv_sock=createServSock();
	cout<<"cteate="<<serv_sock<<endl;
	cout<<"bind="<<bindServSock(serv_sock, "127.0.0.1", 1234)<<endl;
    cout<<"listen"<<listenServSock(serv_sock, 0)<<endl;
	while(1){
		cout<<"I am listening"<<endl;
		clnt_sock=createClntSock(serv_sock);
		cout<<"create="<<clnt_sock<<endl;
		pthread_create(&pth[i++], NULL, communication, &clnt_sock);
		//cout<<"I got "+ss.readInfo(clnt_sock, 10)<<endl;
		//ss.writeInfo(clnt_sock,"hahaha");
		//ss.closeClnt(clnt_sock);
	}
	closeServ(serv_sock);
}*/
