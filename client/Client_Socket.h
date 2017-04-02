#ifndef _CLIENTSOCKET_H
#define _CERVERSOCKET_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string>
#include <iostream>
#include <errno.h>

using namespace std;

		int createClntSock(){
			int sock=socket(AF_INET, SOCK_STREAM, 0);
			return sock;
		}
		int connectServ(int sock, string IP, int port){
			sockaddr_in serv_addr;
    		memset(&serv_addr, 0, sizeof(serv_addr));
    		serv_addr.sin_family = AF_INET;
			char* ch=(char*)IP.c_str();
    		serv_addr.sin_addr.s_addr = inet_addr(ch);
    		serv_addr.sin_port = htons(port);
    		return connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
		}

		int my_read(int fd,char *buffer,int length){
			int bytes_left;
			int bytes_read;
			char *ptr;
			ptr=buffer;
   
			bytes_left=length;
			while(bytes_left>0){
   				bytes_read=read(fd,ptr,bytes_left);
   				if(bytes_read<0){
     				if(errno==EINTR)
        				bytes_read=0;
     				else
        				return(-1);
   				}
   				else if(bytes_read==0)
       				break;
    		bytes_left-=bytes_read;
    		ptr+=bytes_read;
			}
			return(length-bytes_left);
		}
		string readInfo(int sock, int size){
			char buffer[size];
			my_read(sock, buffer, size);
			string str=buffer;
			return str;
		}
		int my_write(int fd,char *buffer,int length){
			int bytes_left;
			int written_bytes;
			char *ptr;
 
			ptr=buffer;
//			cout<<ptr<<endl;
			bytes_left=length;
			int i=0;
			while(bytes_left>0){
//				cout<<"i="<<i++<<"length="<<length<<endl;
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
				//	cout<<"bytes_left="<<bytes_left<<endl;
			}
				return(0);
		}
		void writeInfo(int sock, string str){
			char *buffer=(char *)str.c_str();
//			cout<<buffer<<endl;
//			cout<<strlen(buffer)<<endl;
			my_write(sock, buffer, strlen(buffer));
		}

		void closeClnt(int sock){
			close(sock);
		}

#endif
/*int main(int argc, char* argv[]){
	int sock;
	string str;
	while(1){
		sock=createClntSock();
		cout<<"create="<<sock<<endl;
		cout<<"connect="<<connectServ(sock, "127.0.0.1", 1234)<<endl;
		cout<<"Input something to server"<<endl;
		cin>>str;
		writeInfo(sock, str);
		cout<<"I got "+readInfo(sock, 60)<<endl;
		closeClnt(sock);
	}
}*/
