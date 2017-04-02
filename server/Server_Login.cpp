#include <stdio.h>
#include <string.h>
#include <mysql/mysql.h>
#include <string>
#include <iostream>
#include <pthread.h>
#include "Server_Socket.h"
#include "Player.h"

using namespace std;

//const string IP="127.0.0.1";
//const int PORT=4321;
const int SIZE=30;
const int MAX=10;
const string SEPARATE="\n"; 

int serv_sock;
bool available[MAX];
pthread_t pth[MAX];
pthread_mutex_t lock;

string readHead(string str){
	return str.substr(0,1);
}
string readBody(string str){
	int pos=str.find(SEPARATE);
	return str.substr(1, pos-1);
}
string writeTotal(string head, string body){
	string info=head+body;
	for(int i=info.size(); i<SIZE; i++){
		info+=SEPARATE;
	}
	return info;
}

int getAvailable(bool available[MAX]){
	pthread_mutex_lock(&lock);
	for(int i=0; i<MAX; i++){
		if(!available[i]){
			available[i]=1;
			pthread_mutex_unlock(&lock);
			return i;
		}
	}
	pthread_mutex_unlock(&lock);
	cout<<"Server is full."<<endl;
	return -1;
}

int startServer(char* IP, int PORT){
	pthread_mutex_init(&lock, NULL);
	serv_sock=createServSock();
	cout<<serv_sock<<endl;
	if(serv_sock==-1){
		cout<<"Fail to create."<<endl;
		return -1;
	}
	if(bindServSock(serv_sock, IP, PORT)==-1){
		cout<<"Fail to bind."<<endl;
		return -1;
	}
	if(listenServSock(serv_sock, MAX)==-1){
		cout<<"Fail to listen."<<endl;
		return -1;
	}
	return 0;
}
int getClient(){
	//cout<<serv_sock<<endl;
	int fd=createClntSock(serv_sock);
	if(fd==-1){
		cout<<"Fail to accept."<<endl;
		return -1;
	}
	cout<<"get client "<<fd<<endl;
	return fd;
}
void *run(void * argv){
	int client=(int)(*((int *)argv));
	string head, body, read;
	string name, password, email;
	read=readInfo(client, SIZE);
	head=readHead(read);
	body=readBody(read);
	cout<<"head="<<head<<"   body="<<body<<endl;
//	cout<<head.compare("i")<<endl;
//	cout<<head.compare("u")<<endl;
	if(head.compare("i")==0){
		cout<<"Sign in"<<endl;
		read=readInfo(client, SIZE);
		head=readHead(read);
		body=readBody(read);
		cout<<"head="<<head<<"   body="<<body<<endl;
		writeInfo(client, writeTotal("s", ""));
		closeClnt(client);
	}
	else{
		if(head.compare("u")==0){
			cout<<"Sign Up"<<endl;
			readInfo(client, SIZE);
			readInfo(client, SIZE);
			writeInfo(client, writeTotal("s", ""));
	}
		else{
			cout<<"One client exit.\n"<<endl;
			closeClnt(client);
		}
	}
//	pthread_mutex_lock(&lock);
//	available[client]=0;
//	pthread_mutex_unlock(&lock);
}


int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Usage: %s <IP> <Port #>\n", argv[0]);
		exit(0);
	}
	char* IP=argv[1];
	int PORT;
	sscanf(argv[2], "%d", &PORT);
	if(startServer(IP, PORT)==-1){
		return -1;
	}
	int client, rc;
	for(int i=0; i<MAX; i++){
		available[i]=0;
	}

	while(1){
		client=getClient();
		if(client==-1){
			return -2;
		}
		int a=getAvailable(available);
			if(a==-1){
				return -3;
			}
		rc = pthread_create(&pth[a], NULL, run, (void *)(&client));
        if (rc){
        	printf("ERROR; return code is %d\n", rc);
        	return EXIT_FAILURE;
       	}

	}
}
