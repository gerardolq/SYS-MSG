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
	buf rcvA;
	buf rcvB;
	buf rcvC;
	int msgRec = 0;
	
	while(rcvA.greeting[0]!='T' || msgRec < 10001){

		if(msgRec > 10000)
		{
			msg.m_type = 3;
			cout << "WE HIT 10000!!!!" << endl;
			strncpy(msg.greeting, "T", size);
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			cout<<"booooo"<<endl;
			msgRec++;
		}

		if(msgrcv(qid, (struct msgbuf *)&rcvA, size, 997, IPC_NOWAIT) != -1)
		{
			//cout <<"DataHub Receiving from ProbeA (process ID): " << getpid() << endl;
			//cout << rcvA.greeting << endl << endl;
			//msgRec++;
			msg.m_type = 4;
			strncpy(msg.greeting, rcvA.greeting, size);
			if(rcvA.greeting[0] != 'T')
				msgsnd(qid, (struct msgbuf *)&msg, size, 0); // sending to probeA
		}

		if(msgrcv(qid, (struct msgbuf *)&rcvB, size, 257, IPC_NOWAIT) != -1)
		{
			cout <<"DataHub Receiving from ProbeB (process ID): " << getpid() << endl;
			cout << "message " << msgRec;
			cout << rcvB.greeting << endl << endl;
			msgRec++;
		}

	
	}

	//msgrcv(qid, (struct msgbuf *)&rcv, size, 997, 0);
	//cout << getpid() << endl;
	//cout << rcv.greeting << endl;
	msgctl(qid,IPC_RMID,NULL);

	return 0;
}