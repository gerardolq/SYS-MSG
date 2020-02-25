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

int main(){
	int rho = 2001463;
	srand(time(NULL));
	int qid = msgget(ftok(".",'u'), 0);
	buf msg;
	string message;
	int size = sizeof(msg) - sizeof(long);
	msg.m_type = 90;
	int num = getpid();
	message = toString(num);
	strncpy(msg.greeting, message.c_str(), size);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	strncpy(msg.greeting, "kill", size);
	kill_patch(qid, (struct msgbuf *)&msg,size, 1);
	msg.m_type = 90;

	while (true){ //WHILE IT IS NOT "TERMINATE"
		num = rand();
		if(num % rho == 0)
			{
				message = toString(num);
				strncpy(msg.greeting, message.c_str(), size);
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}
	}

}















