#ifndef _CCHESSBOARD_H
#define _CCHESSBOARD_H

#include <iostream>
#include <stdlib.h>
#include <string>
#include <bitset>
#include <stdio.h>
#include "Player.h"

using namespace std;

const string BBLUE="44";
const string CBLACK="30";
const string CWHITE="37";
const int LEN=12;
const string CB="●";
const string CW="\033["+BBLUE+";"+CBLACK+"m●\033[0m\e["+BBLUE+"m";
const string TL="┏";
const string TM="┯";
const string TR="┓";
const string BL="┗";
const string BM="┷";
const string BR="┛";
const string ML="┠";
const string MM="┼";
const string MR="┨";
const string HC="─";
const string VC="│";
const string HB="━";
const string VB="┃";

class Client_Chessboard{
	private:
		bitset<LEN*LEN*2> chessCode;

	public:
	
	Client_Chessboard(){
		for(int i=0; i<LEN*LEN*2; i++){
			chessCode[i]=0;
		}
	}
	void makeEmpty(){
		for(int i=0; i<LEN*LEN*2; i++){
			chessCode[i]=0;
		}
	}
	int getCodePosition(int x, int y, int offset){
		return (LEN*y+x)*2+offset;
	}

	bool  leftToRight(int x, int y, int offset){
		int num1=0;
		int num2=0;
		int i=1;
//		printf("offset=%d\n", offset);
		while((x+i<LEN)&&chessCode[getCodePosition(x+i, y, offset)]){
			num1++;
			i++;
		}
		i=1;
		while((x-i>-1)&&chessCode[getCodePosition(x-i, y, offset)]){
			num2++;
			i++;
		}
		printf("num1=%d, num2=%d\n", num1, num2);
		if((num1+num2)>=4){
			return 1;
		}
		else{
			return 0;
		}
	}
	bool topToBottom(int x, int y, int offset){
		int num1=0;
		int num2=0;
		int i=1;
		while((y+i<LEN)&&chessCode[getCodePosition(x, y+i, offset)]){
			num1++;
			i++;
		}
		i=1;
		while((y-i>-1)&&chessCode[getCodePosition(x, y-i, offset)]){
			num2++;
			i++;
		}
		printf("num1=%d, num2=%d\n", num1, num2);
		if((num1+num2)>=4){
			return 1;
		}
		else{
			return 0;
		}

	}
	bool lefttopToRightbottom(int x, int y, int offset){
		int num1=0;
		int num2=0;
		int i=1;
		while((x+i<LEN)&&(y+i<LEN)&&chessCode[getCodePosition(x+i, y+i, offset)]){
			num1++;
			i++;
		}
		i=1;
		while((x-i>-1)&&(y-i>-1)&&chessCode[getCodePosition(x-i, y-i, offset)]){
			num2++;
			i++;
		}
		printf("num1=%d, num2=%d\n", num1, num2);
		if((num1+num2)>=4){
			return 1;
		}
		else{
			return 0;
		}
	}
	bool righttopToLeftbottom(int x, int y, int offset){
		int num1=0;
		int num2=0;
		int i=1;
		while((x-i>-1)&&(y+i<LEN)&&chessCode[getCodePosition(x-i, y+i, offset)]){
			num1++;
			i++;
		}
		i=1;
		while((x+i<LEN)&&(y-i>-1)&&chessCode[getCodePosition(x+i, y-i, offset)]){
			num2++;
			i++;
		}
		printf("num1=%d, num2=%d\n", num1, num2);
		if((num1+num2)>=4){
			return 1;
		}
		else{
			return 0;
		}
	}

	bool checkWin(string position, bool player){
		int p=getCodePosition(position);
		int offset=0;
		bool win=false;//the win status
		if(player){
			offset=1;
		}
		int x=(p/2)%12;
		int y=(p/2)/12;
		printf("x=%d, y=%d\n", x, y);
		return topToBottom(x, y, offset)||leftToRight(x, y, offset)||lefttopToRightbottom(x, y, offset)||righttopToLeftbottom(x, y, offset);
	}

//	int to string	
	string int_to_string(int i)  {  
    	char ch[10];  
    	sprintf(ch, "%d", i);  
    	string s(ch);  
  
    	return s;  
	}  


	void setChessCode(string code){
		string p=code.substr(1,(LEN*LEN)/4);
//		cout<<p<<endl;
		int index=0;
		for(int i=0; p[i]!=0; i++){
//			cout<<p[i]<<endl;
			bitset<8> temp(static_cast<unsigned long>(p[i]));
			for(int j=0; j<8; j++){
//				cout<<index<<endl;
				chessCode[index++]=temp[j];
			}
		}
	}
// 		NEED STRING HEAD!!!!!!!!!!!!!!!!!!
	string convertToString(){
		string str=" ";
		for(int i=0; i<LEN*LEN/4; i++){
			bitset<8> temp;
			for(int j=0; j<8; j++){
				temp[j]=chessCode[i*8+j];
			}
			str+=(char)(temp.to_ulong());
		}
		str+=" ";
		return str;
	}

	 bitset<LEN*LEN*2> getChessCode(){
		 //cout<<chessCode;
		return chessCode;
	}

	void setBackGround(){
		cout<<"\e["+BBLUE+"m"<<endl;
	}

	bool is_num(char a){
		return (a>=48&&a<=57);
	}

	bool is_upChar(char a){
		return (a>=65&&a<=90);
	}

	bool is_lowChar(char a){
		return (a>=97&&a<=122);
	}

	int getCodePosition(string position){
		char* p=(char*)position.c_str();
		bool s=1;
		int x,y;
		if(is_lowChar(p[0])||is_upChar(p[0])){
			if(is_lowChar(p[0])){
				x=p[0]-97;
			}
			else{
				x=p[0]-65;
			}
			if(p[2]=='\0'){
				if(is_num(p[1])){
					y=p[1]-48;
				}
				else{
					s=0;
				}
			}
			else{
				if(is_num(p[1])&&is_num(p[2])){
					y=(p[1]-48)*10+(p[2]-48);
				}
				else{
					s=-1;
				}
			}
		}
		else{
			if(is_num(p[0])){
				if(is_num(p[1])){
					y=(p[0]-48)*10+(p[1]-48);
					if(is_upChar(p[2])){
						x=p[2]-65;
					}
					else{
						if(is_lowChar(p[2])){
							x=p[2]-97;
						}
						else{
							s=-2;
						}
					}
				}
				else{
					if(is_upChar(p[1])){
						x=p[1]-65;
						y=p[0]-48;
					}
					else{
						if(is_lowChar(p[1])){
							x=p[1]-97;
							y=p[0]-48;
						}
					}
				}
			}
			else{
				s=-3;
			}
		}
		int r;
		if(s){
			r=--y*LEN*2+--x*2;
		}
		else{
			r=-1;
		}
//		cout<<"x="<<x<<endl;
//		cout<<"y="<<y<<endl;
		return r;
	}

	bool judge(string position){
		int p=getCodePosition(position);
	//	cout<<p<<endl;
		return !(chessCode[p]||chessCode[++p]);
	}

	bool play(string position, bool player){
		if(judge(position)){
			if(!player){
				chessCode[getCodePosition(position)]=1;
			}
			else{
				chessCode[getCodePosition(position)+1]=1;
			}
						return 1;
		}
		else{
			return 0;
		}
	}

	string printBoard(){
		string HSPACE=HC+HC+HC;       //horizontal space
		string HSPACEB=HB+HB+HB;      //horizontal bold space
		string VSPACE=VB+"   ";       //vertical space
		string LEFTS="\t\t\t";
		string LEFTS2="\t\t";

		//intitialize VSPACE
		for(int i=0; i<LEN; i++){
			VSPACE+=(VC+"   ");
		}
		VSPACE+=(VB+"\n");

		//create chess board
		int count=0;

		//create top board and Horizontal axis
		string board=LEFTS;
		char ha='a';
		for(int i=0; i<LEN+2; i++){
			board+=(ha++);
			board+="   ";
		}
		board+="\n";
		board+=(LEFTS2+"     0\t"+TL+HSPACEB);
		for(int i=0; i<LEN; i++){
			board+=(TM+HSPACEB);
		}
		board+=(TR+"\n");

		//create middle board
		board+=LEFTS+VSPACE;
		for(int i=0; i<LEN; i++){
			board+=(LEFTS2+"     "+int_to_string(i+1)+"\t"+ML+HSPACE);
			for(int j=0; j<LEN; j++){
				if(chessCode[count++]==0){
					if(chessCode[count++]==0){
						board+=(MM+HSPACE);
					}
					else{
						board+=(CW+HSPACE);
					}
				}
				else{
					if(chessCode[count++]==0){
						board+=(CB+HSPACE);
					}
					else{
						board+=("X"+HSPACE);
					}
				}
			}
			board+=(MR+"\n");
			board+=(LEFTS+VSPACE);
		}

		//create bottom board
		board+=(LEFTS2+"     "+int_to_string(LEN+1)+"\t"+BL+HSPACEB);
		for(int i=0; i<LEN; i++){
			board+=(BM+HSPACEB);
		}
		board+=(BR+"\n");
		return board;
	}
};

#endif
		/*int main(int argc, char *argv[]){
			Player p[2];
			p[0].setFirstHand(1);
			p[1].setFirstHand(0);
			Server_Chessboard scb(p[0], p[1]);
			scb.setBackGround();
			string ss;
			int i=0;
			while(1){
				cout<<"input:"<<endl;
				cin>>ss;
				bool r=scb.play(ss, p[i%2]);
				cout<<r<<endl;
				string str=scb.printBoard();
				cout<<str;
				if(scb.checkWin(ss, p[i%2])){
					printf("WIN!!!\n");
				}
				i++;
			}
		}*/
