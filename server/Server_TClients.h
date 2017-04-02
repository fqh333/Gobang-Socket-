#ifndef _STCS_H
#define _STCS_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include "Server_Match.h"

using namespace std;

class Server_TClients{
	private:
		int status;
		int first;
		int second;
		Server_Match sm;
	public:
		Server_TClients(){
			status=0;
		}
		void makeEmpty(){
			status=0;
		}
		int addClient(int fd, Player p){
			if(status==0){
				status=1;
				first=fd;
				sm.setFPlayer(p);
				return 1;
			}
			else{
				if(status==1){
					status=2;
					second=fd;
					sm.setSPlayer(p);
					sm.makeEmpty();
					return 2;
				}
				else{
					cout<<"error: too many players"<<endl;
					return -1;
				}
			}
		}
		int checkStatus(){
			return status;
		}
		
		int getFirst(){
			return first;
		}

		int getSecond(){
			return second;
		}

		bool checkWin(string position){
			return sm.checkWin(position);
		}

		bool play(string position){
			return sm.play(position);
		}
		string printBoard(){
			return sm.printBoard();
		}
		void setBackGround(){
			sm.setBackGround();
		}
		string nowPlay(){
			return sm.nowPlay();
		}
};

/*int main(int argc, char *argv[]){
	Server_TClients sts;
	cout<<sts.checkStatus()<<endl;
	Player p1("qqwe", "", "", 0,0), p2("222","", "",0,0);
	sts.addClient(1, p1);
	sts.addClient(2,p2);
	sts.setBackGround();
	string str;
	while(1){
		cout<<sts.printBoard()<<endl;
		cin>>str;
		sts.play(str);
		if(sts.checkWin(str)){
			cout<<sts.nowPlay()<<"WIN!!!!!"<<endl;
		}
	}
	return 1;
}*/

#endif
