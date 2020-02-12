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
	int alpha = 997;
	srand(time(NULL));
	int qid = msgget(ftok(".",'u'), 0);
	int num=49;
	buf msg;
	buf rcv;
	string message;
	int size = sizeof(msg) - sizeof(long);
	msg.m_type = 997;
	do
	{
		//num = rand();
		num = 997;
		message = toString(num);
		strncpy(msg.greeting, message.c_str(), size);
		if(num % alpha == 0)
			{
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
		num = 49;

	}while(num > 50);
	msgrcv(qid, (struct msgbuf *)&rcv, size, 4, 0);
	cout << rcv.greeting << endl;
	strncpy(msg.greeting, "TERMINATE", size);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	return 0;
}