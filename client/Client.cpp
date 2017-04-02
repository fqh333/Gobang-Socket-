#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include "Client_Chessboard.h"
#include "Client_Socket.h"
#include "Player.h"

using namespace std;

//const string IP="127.0.0.1";
//const int PORT=1234;
const int SIZE=30;
const string SEPARATE="\n";

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

int waitServer(int client, Player *player){
	cout<<"Wait server."<<endl;

	string info=readInfo(client, SIZE);
	string head=readHead(info);
	string body=readBody(info);
	cout<<body<<endl;
	if(head=="1"){
		cout<<"You are first player."<<endl;
		player->setFirstHand(1);
		info=readInfo(client, SIZE);
		body=readBody(info);
		cout<<body<<endl;
		return 0;
	}
	else{
		if(head=="2"){
			cout<<"You are second player."<<endl;
			player->setFirstHand(0);
			return 0;
		}
		cout<<"Data error."<<endl;
		return -1;
	}
}
bool judgeHead(string str){
	string head=readHead(str);
	if(!head.compare("w")){
		cout<<"You win."<<endl;
		return 1;
	}
	if(!head.compare("l")){
		cout<<"You lose."<<endl;
		return 1;
	}
	if(!head.compare("e")){
		cout<<"Your opponent exited, and you are winner"<<endl;
		return 1;
	}
	return 0;
}

int firstPlayer(int client, Client_Chessboard* ccb){
		string str;
		cout<<"It's your turn."<<endl;
		cin>>str;
		if(!str.compare("exit")){
			writeInfo(client, writeTotal("e", "lose."));
			return -1;
		}
		while(!ccb->judge(str)){
			cout<<"This position is not available. \nInput again."<<endl;
			cin>>str;
		}
		ccb->play(str, 1);
		cout<<ccb->printBoard();
		writeInfo(client, writeTotal("n", str));
		cout<<"Wait server."<<endl;
		str=readInfo(client, SIZE);
		if(judgeHead(str)){
			return 1;
		}
		ccb->play(readBody(str), 0);
		cout<<ccb->printBoard();
		return 0;
		//cout<<readBody(str)<<endl;
}
int secondPlayer(int client, Client_Chessboard* ccb){
		string str;
		cout<<"Wait server."<<endl;
		str=readInfo(client, SIZE);
		if(judgeHead(str)){
			return 1;
		}
		ccb->play(readBody(str), 1);
		cout<<ccb->printBoard();
		//cout<<str<<endl;
		//cout<<readBody(str)<<endl;
		cout<<"It's your turn"<<endl;
		cin>>str;
		if(!str.compare("exit")){
			writeInfo(client, writeTotal("e", "lose."));
			return -1;
		}
		while(!ccb->judge(str)){
			cout<<"This position is not available. \nInput again."<<endl;
			cin>>str;
		}
		ccb->play(str, 0);
		cout<<ccb->printBoard();
		writeInfo(client, writeTotal("n", str));
		return 0;
}

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("Usage: %s <IP> <Port #>\n", argv[0]);
		exit(0);
	}
	char* IP=argv[1];
	int PORT;
	sscanf(argv[2], "%d", &PORT);


	Player player;
	player.setName("player");
	

	Client_Chessboard ccb;
	ccb.setBackGround();
	int client;
	client=createClntSock();
	if(client==-1){
		cout<<"Fali to create."<<endl;
		return -1;
	}
	if(connectServ(client, IP, PORT)==-1){
		cout<<"Fail to connect."<<endl;
		return -2;
	}

	waitServer(client, &player);
	
//	cout<<player.getFirstHand()<<endl;
	if(player.getFirstHand()){
		cout<<ccb.printBoard();
		while(1){	
			if(firstPlayer(client, &ccb)!=0){
				break;
			}
		}
	}
	else{
		cout<<ccb.printBoard();
		while(1){
			if(secondPlayer(client, &ccb)!=0){
				break;
			}
		}
	}
}
