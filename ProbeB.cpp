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
	int beta = 1777;
	srand(time(NULL));
	int qid = msgget(ftok(".",'u'), 0);
	buf msg;
	string message;
	int size = sizeof(msg) - sizeof(long);
	msg.m_type = 100;
	int num = getpid();
	message = toString(num);
	strncpy(msg.greeting, message.c_str(), size);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);

	while (true){ //WHILE IT IS NOT "TERMINATE"
		num = rand();
		if(num % beta == 0)
			{
				cout << "Message sent from: " << getpid() << endl;
				message = toString(num);
				strncpy(msg.greeting, message.c_str(), size);
				cout << "Number sent:" << num << endl;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
	}

	cout << "Last number: " << num<< endl;


	//msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	return 0;
}