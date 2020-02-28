// Written by: Eunice Kang, Gerardo Lopez
// File Name: ProbeA.cpp
// Description: Generates a random number that is divisible by Alpha
// 		Sends the random number to DataHub
//		Waits for acknowledgement from DataHub in the form of a message
//		Is terminated if it generates a random number less than 50

#include<iostream>
#include<time.h>
#include<cstdlib>
#include<unistd.h>
#include<sstream>
#include<sys/types.h>
#include<cstring>
#include <sys/ipc.h> 
#include <sys/msg.h>
using namespace std;


//Our message struct
struct buf
{
	long m_type;
	char greeting[50];
};


// FUNCTION: TOSTRING
// input: int n - number that we want to convert to a string
// output: string
// function: converts the number n into a string of chars
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
int main()
{
	//setting our alpha
	int alpha = 2001469;
	
	//seeding our random number generator to time
	srand(time(NULL));
	
	//grabing the id of the queue
	int qid = msgget(ftok(".",'u'), 0);
	
	//setting up message sending variables --------------------
	//creating a send message and a receive message
	buf msg;
	buf rcv;
	
	//what stores the string
	string message;
	//setting the size
	int size = sizeof(msg) - sizeof(long);
	//setting our m type
	msg.m_type = 257;
	//num will be used to hold the number that may or
	//may not be sent to data hub
	int num;
	//set up complete --------------------------------------------
	
	//sending the PID to DataHUb
	num = getpid();
	message = toString(num);
	strncpy(msg.greeting, message.c_str(), size);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	//PID sent to data hub
	
	do
	{
		//grab new input
		num = rand();
		//validate input
		if(num % alpha == 0)
			{
				//begin to send message upon valid input
				message = toString(num);
				strncpy(msg.greeting, message.c_str(), size);
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);	// sends to DataHub
				msgrcv(qid, (struct msgbuf *)&rcv, size, 20000, 0);	// receives from DataHub
				//once message validation received, move on
			}
	//if our input number is smaller than 50 we quit
	}while(num >= 50);

	//Notify DataHub that ProbeA is terminating
	strncpy(msg.greeting, "T", size);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	
	//Termination - Good Bye
	return 0;
}
