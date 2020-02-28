#include<iostream>
#include<time.h>
#include<cstdlib>
#include<unistd.h>
#include<sstream>
#include<sys/types.h>
#include<cstring>
#include <sys/ipc.h> 
#include <sys/msg.h>

#include "KILL_PATCH.H"

using namespace std;


//Our message struct
struct buf
{
	long m_type;
	char greeting[50];
};

//used to convert our numbers to a string so that
//we can send it DataHub
string toString(int n)
{
	string s = "";
	char c;
	int last;
	while(n > 0)
	{
		last = n%10;
		c =  '0' +last;
		s = c + s;
		n = n/10;
	}

	return s;
}

//--------------------------------------------------------------------------//
//------------------------------MAIN function-------------------------------//
//--------------------------------------------------------------------------//
int main(){
	//setting up our rho
	int rho = 2001463;
	
	//seeding out rand with time input
	srand(time(NULL));
	
	//grabbing id of the queue
	int qid = msgget(ftok(".",'u'), 0);
	
	//---------------------setting up message sending variables ---------------------
	//-message send variable
	buf msg;
	//string used to copy number input to our msg
	string message;
	//setting up size
	int size = sizeof(msg) - sizeof(long);
	//setting up m type
	msg.m_type = 90;
	//number used to hold input
	int num;
	//---------------------set up complete ---------------------------------
	
	//---------------------sending the pid to Data Hub ---------------------------
	num = getpid();
	message = toString(num);
	strncpy(msg.greeting, message.c_str(), size);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	//---------------------PID sent to Data Hub ----------------------------------
	
	//------------------Setting up Killpatch with "kill" message -------------------
	strncpy(msg.greeting, "kill", size);
	kill_patch(qid, (struct msgbuf *)&msg,size, 1);
	//---------------------Done setting up killpatch -------------------------
	

	while (true)
	{ //WHILE IT IS NOT "TERMINATE"
		//grab new input
		num = rand();
		//validate input
		if(num % rho == 0)
		{
			//----------------------------begin to send message ------------------------
			//translate our number into a string
			message = toString(num);
			//wrap our message up nicely
			strncpy(msg.greeting, message.c_str(), size);
			//send to DATA HUB
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			//-----------------------------message sent-----------------------------
		}
	}
	
	// Adíos ...
	return 0;
}















