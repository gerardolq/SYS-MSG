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
	int beta = 257;
	srand(time(NULL));
	int qid = msgget(ftok(".",'u'), 0);
	int num=49;
	//
	buf msg;
	//
	buf rcv;
	rcv.greeting[0] = '1';
	string message;
	int size = sizeof(msg) - sizeof(long);
	msg.m_type = 257;
	while (rcv.greeting[0] != 'T'){ //WHILE IT IS NOT "TERMINATE"
		msgrcv(qid, (struct msgbuf *)&rcv, size, 3, IPC_NOWAIT);
		//num = rand();
		num = beta;
		//cout << "i here!!!" << endl;
		if(num % beta == 0)
			{
				message = toString(num);
				strncpy(msg.greeting, message.c_str(), size);
				cout << "Number sent:" << num << endl;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);	// sends to DataHub
			}
	}
	cout << "Last number: " << num<< endl;


	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	return 0;
}