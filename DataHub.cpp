#include<iostream>
#include<time.h>
#include<cstdlib>
#include<unistd.h>
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

int main()
{
	int beta = 257;
	int rho = 251;
	int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);
	buf msg;
	int size = sizeof(msg) - sizeof(long);
	buf rcv;
	msg.m_type = 4;
	strncpy(msg.greeting, "Fake message",size);
	msgsnd(qid, (struct msgbuf *)&msg, size, 0);
	msgrcv(qid, (struct msgbuf *)&rcv, size, 997, 0);
	cout << getpid() << endl;
	cout << rcv.greeting << endl;
	msgrcv(qid, (struct msgbuf *)&rcv, size, 997, 0);
	cout << getpid() << endl;
	cout << rcv.greeting << endl;
	msgctl(qid,IPC_RMID,NULL);

	return 0;
}