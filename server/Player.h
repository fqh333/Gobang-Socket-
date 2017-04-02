#ifndef _PLAYER_H
#define _PLAYER_H

#include <iostream>
#include <string>

using namespace std;

class Player{
	private:
		string name;
		string email;
		string password;
		bool firstHand;
		int roundW;
		int roundT;
	public:
		Player(){
			name="";
			email="";
			password="";
		}
		Player(string n, string e, string p, int w, int t){
			name=n;
			email=e;
			password=p;
			roundW=w;
			roundT=t;
		}

		bool isEmpty(){
			return (name=="");
		}
		void setFirstHand(bool fh){
			firstHand=fh;
		}
		bool getFirstHand(){
			return firstHand;
		}
		void setPlayer(string n, string e, string p, int w, int t){
			name=n;
			email=e;
			password=p;
			roundW=w;
			roundT=t;
		}
		void setName(string str){
			name=str;
		}
		string getName(){
			return name;
		}
		string getEmail(){
			return email;
		}
		string getPassword(){
			return password;
		}
		int getRoundW(){
			return roundW;
		}
		int getRoundT(){
			return roundT;
		}
};

#endif
