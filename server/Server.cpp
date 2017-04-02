#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <pthread.h>
#include "Server_Socket.h"
#include "Player.h"
#include "Server_TClients.h"

//const string IP="127.0.0.1";
//const int PORT=1234;
const int MAX=10;
const int SIZE=30;
const string SEPARATE="\n"; 

int serv_sock;
Server_TClients sts[MAX];
pthread_t pth[MAX];
pthread_mutex_t lock;

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
int getAvailable(){
	pthread_mutex_lock(&lock);
	for(int i=0; i<MAX; i++){
		if(sts[i].checkStatus()==1){
			pthread_mutex_unlock(&lock);
			cout<<"Find a room with one player."<<endl;
			return i;
		}
	}
	for(int i=0; i<MAX; i++){
		if(sts[i].checkStatus()==0){
			pthread_mutex_unlock(&lock);
			cout<<"Create a new room."<<endl;
			return i;
		}
	}
	pthread_mutex_unlock(&lock);
	cout<<"Fail to find an available room."<<endl;
	return -1;
}
int addClient(int r, int fd, Player p){

	pthread_mutex_lock(&lock);
	int res=sts[r].addClient(fd, p);
	pthread_mutex_unlock(&lock);
	return res;
}
bool commnuicate(int first,int second){
	string info, head, body;
	info=readInfo(first, SIZE);
	//cout<<read<<endl;
	body=readBody(info);
	head=readHead(info);
	if(!head.compare("e")){
		writeInfo(second, writeTotal("e", "lose"));
		return 1;
	}
	pthread_mutex_lock(&lock);
	sts[MAX].play(body);
	if(sts[MAX].checkWin(body)){
		head="w";
		writeInfo(first, writeTotal(head, body));
		head="l";
		writeInfo(second, writeTotal(head, body));
		return 1;
	}
	else{
		head="n";
	}
	pthread_mutex_unlock(&lock);
	writeInfo(second, writeTotal(head, body));
	return 0;
}

void *runRoom(void * argv){
	int room= (int)(*((int *)argv));
	string read, write;
	pthread_mutex_lock(&lock);
	int first=sts[room].getFirst();
	int second=sts[room].getSecond();
	pthread_mutex_unlock(&lock);
	writeInfo(first, writeTotal("2", "Game will begin."));
	writeInfo(second, writeTotal("2", "Game will begin."));

	while(1){
		if(commnuicate(first, second)){
			break;
		}
		if(commnuicate(second, first)){
			break;
		}
//		cout<<"111111"<<endl;
/*		read=readInfo(first, SIZE);
		//cout<<read<<endl;
		body=readBody(read);
		head=readHead(read);

		pthread_mutex_lock(&lock);
		sts[MAX].play(body);
		if(sts[MAX].checkWin(body)){
			;
		}
		pthread_mutex_unlock(&lock);

		write=read;
		writeInfo(second, write);

//		cout<<"222222"<<endl;
		read=readInfo(second, SIZE);
		//cout<<read<<endl;
		write=read;
		writeInfo(first, write);*/
	}
	pthread_mutex_lock(&lock);
	sts[room].makeEmpty();
	pthread_mutex_unlock(&lock);
	closeClnt(first);
	closeClnt(second);
}


int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Usage: %s <IP> <Port #>\n", argv[0]);
		exit(0);
	}
	char* IP=argv[1];
	int PORT;
	sscanf(argv[2], "%d", &PORT);
/*	string str="abcdefg\n\n\ndasdas";
	cout<<str<<endl;
	cout<<readHead(str)<<endl;
	cout<<readBody(str)<<endl;
	cout<<writeTotal("a", "bcdef");
	*/

	int client;
	int room;
	int rc;


	Player p[4];
	p[0].setName("001");
	p[1].setName("002");
	p[2].setName("003");
	p[3].setName("004");
	int num=0;

	if(startServer(IP, PORT)==-1){
		return -1;
	}
	while(1){
		client=getClient();
		if(client==-1){
			return -2;
		}
		room=getAvailable();
		if(room==-1){
			return -3;
		}
		
		int temp=addClient(room, client, p[num++]);
		cout<<"temp="<<temp<<endl;
		switch (temp){
			case 1:
				writeInfo(client, writeTotal("1", "Wait one more player."));
				break;
			case 2:
				rc = pthread_create(&pth[room], NULL, runRoom, (void * )(&room));
        		if (rc){
            		printf("ERROR; return code is %d\n", rc);
            		return EXIT_FAILURE;
       			 }
				break;
			default:
				return -4;
		}
	}
	return 1;
}
