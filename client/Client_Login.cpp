#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include "Client_Socket.h"
#include "Player.h"

using namespace std;

const int SIZE=30;
const string SEPARATE="\n";
//const string IP="127.0.0.1";
//const int PORT=4321;

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
bool clientExit(int sock){
	writeInfo(sock, writeTotal("e", " "));
}

bool SignIn(int sock){
	string name, password, res, head;
	cout<<"\t\tPlease input your username:"<<endl;
	cin>>name;
	writeInfo(sock, writeTotal("i", name));
	cout<<"\t\tPlease input your password:"<<endl;
	cin>>password;
	writeInfo(sock, writeTotal("i", password));
	res=readInfo(sock, SIZE);
	head=readHead(res);
	//cout<<"result="<<head<<endl;
	if(head.compare("s")==0){
		return 1;
	}
	else{
		return 0;
	}
}

bool SignUp(int sock){
	string name, password, email, password2;
		// Username Input
		while(1){
        	cout<<"\t\tPlease input your username (no more than 10 chars): "<<endl;
	        cin>>name;
        
    	    // To judge if the username is valid
    	    if((name.length())<=10){
        	    while(1){
            	    // Password Input
                	cout<<"\t\tPlease input your password:"<<endl;
               		cin>>password;

					cout<<"\t\tPlease retype your password:"<<endl;
					cin>>password2;
                
                // To judge if the psw is valid
                	if((password.length())>6&&password.compare(password2)==0){				                
                		cout<<"\t\tPlease input your Email："<<endl;
               			cin>>email;
                    	cout<<"\n\n\t\tRegister successfully! Your username is "+name+", Your password is "+password<<endl;
						writeInfo(sock, writeTotal("u", name));
						writeInfo(sock, writeTotal("u", password));
						writeInfo(sock, writeTotal("u", email));
						string res=readHead(readInfo(sock, SIZE));
						if(res.compare("s")){
							return 1;
						}
                    	return 0;
                	}
                	else{
                    	cout<<"\n\t\tThe length of PASSWORD is too short or 2 times are not same, please re-enter: "<<endl;
                	}
            	}
			}
		}

}

int main(int argc, char* argv[]){
	if(argc != 4){
		printf("Usage: %s <IP> <Port #>(For Login) <Port #(For Play Game)>\n", argv[0]);
		exit(0);
	}
	char* IP=argv[1];
	int PORT;
	sscanf(argv[2], "%d", &PORT);

	Player player;
	int num;

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
	while(1){
	cout<<"\t\t\tGobang Game\n"<<endl;
        
    cout<<"\t\t\t1:Regiser"<<endl;
    cout<<"\t\t\t2:Login"<<endl;
    cout<<"\t\t\t0:Exit\n"<<endl;
        
        // Identifer Input
    cout<<"\t\tPlease enter the number:"<<endl;;
    cin>>num;
        
        // To judge if the number is 1 or 2 or 0
        switch(num)
        {
            case 1:
				SignUp(client);
				break;
            case 2:
                if(SignIn(client)){
					cout<<"Success"<<endl;
//					return 0;
					//sleep(1);
					execl("./client", "client", argv[1], argv[3], NULL);
				}
				else{
					cout<<"Failed, please try again"<<endl;
				}
                break;
            case 0:
                /* close connection */
//               	return 0;
				clientExit(client);
				return 0;
				break;
                
            default:
                cout<<"\t\tYour number is incorrect, please re-enter: "<<endl;
        }
	}

}
