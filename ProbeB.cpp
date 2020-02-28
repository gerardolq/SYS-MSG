// Written by: Eunice Kang, Gerardo Lopez
// File Name: ProbeB.cpp
// Description: Generates a random number that is divisible by Beta
// 		Sends the random number to DataHub
//		Is terminated by DataHub if it receives 10000 messages

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

int main()
{
	// initialization
	srand(time(NULL));
	int qid = msgget(ftok(".",'u'), 0);
	int beta = 667141;
	buf msg;
	string message;
	int size = sizeof(msg) - sizeof(long);
	msg.m_type = 100;
	
	// Sending the pid to DataHub
	int num = getpid();
	message = toString(num);
	strncpy(msg.greeting, message.c_str(), size);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	// Continuously send messages to DataHub
	while (true){ //WHILE IT HAS NOT BEEN "TERMINATED"
		num = rand();
		if(num % beta == 0)
			{
				message = toString(num);
				strncpy(msg.greeting, message.c_str(), size);
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
	}

	//cout << "Last number: " << num<< endl;


	//msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	return 0;
}
