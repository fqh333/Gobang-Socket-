#ifndef _SM_H
#define _SM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string>
#include <iostream>
#include "Server_Chessboard.h"


using namespace std;

class Server_Match{
	private:
		Server_Chessboard chessboard;
		Player first;
		Player second;
		int Round;
	public:
		Server_Match(){
			Round=-1;
		}
		Server_Match(string n1, string e1, string p1, int w1, int t1, string n2, string e2, string p2, int w2, int t2){
			first.setPlayer(n1,e1,p1,w1,t1);
			second.setPlayer(n2,e2,p2,w2,t2);
			first.setFirstHand(1);
			second.setFirstHand(0);
			makeEmpty();
		}

		Server_Match(Player p1, Player p2){
			first=p1;
			second=p2;
			first.setFirstHand(1);
			second.setFirstHand(0);
		}
		void setServer_Match(string n1, string e1, string p1, int w1, int t1, string n2, string e2, string p2, int w2, int t2){
			first.setPlayer(n1,e1,p1,w1,t1);
			second.setPlayer(n2,e2,p2,w2,t2);
			first.setFirstHand(1);
			second.setFirstHand(0);
			makeEmpty();
		}
		void makeEmpty(){
			Round=0;
			chessboard.makeEmpty();
		}
		void setFPlayer(Player p1){
			first=p1;
			first.setFirstHand(1);
		}
		void setSPlayer(Player p2){
			second=p2;
			second.setFirstHand(0);
		}
		void setServer_Match(Player p1, Player p2){
			first=p1;
			second=p2;
			first.setFirstHand(1);
			second.setFirstHand(0);
			makeEmpty();
		}

		int CheckPlayerStatus(){
			if(first.isEmpty()&&second.isEmpty()){
				return 0;
			}
			else if(second.isEmpty()){
				return 1;
			}
			return 2;
		}
		Server_Chessboard getChessboard(){
			return chessboard;
		}
		Player getFPlayer(){
			return first;
		}
		Player getSPlayer(){
			return second;
		}
		bool checkWin(string position){
			if(Round%2==1){
				return chessboard.checkWin(position, first.getFirstHand());
			}
			else{
				return chessboard.checkWin(position, second.getFirstHand());
			}
		}

		bool play(string position){
			cout<<Round<<endl;
			if(++Round%2==1){		
				cout<<first.getFirstHand()<<endl;
				cout<<second.getFirstHand()<<endl;
				cout<<"---------------"<<endl;
				return chessboard.play(position, first.getFirstHand());
			}
			else{
				cout<<"+++++++++++++++"<<endl;
				return chessboard.play(position, second.getFirstHand());
			}
		}
		string printBoard(){
			return chessboard.printBoard();
		}
		void setBackGround(){
			chessboard.setBackGround();
		}
		Player getPlayerF(){
			return first;
		}
		Player getPlayerS(){
			return second;
		}
		string nowPlay(){
			if(Round%2==1){
				return first.getName();
			}
			return second.getName();
		}
};

/*int main(int argc, char* argv[]){
	Player player1, player2;
	player1.setPlayer("001", "null", "null", 0 ,0);
	player2.setPlayer("002", "null", "null", 0, 0);
	Server_Match sm(player1, player2);
	string str;
	sm.setBackGround();
	while(1){
		cout<<sm.printBoard()<<endl;
		cin>>str;
		sm.play(str);
		if(sm.checkWin(str)){
			return 0;
		}
	}
	return 0;
}*/

#endif		
