#include<iostream>
#include<time.h>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<cstring>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

#include "FORCE_PATCH.H"

using namespace std;

struct buf
{
	long m_type;
	char greeting[50];
};



int main()
{
	int alpha = 5443;
	int beta = 1777;
	int rho = 251;
	int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);
	buf msg;
	msg.m_type = 20000;
	buf rcv;
	int size = sizeof(msg) - sizeof(long);
	bool killed = false;
	//starts at 3 since we first receive a msg from each probe with their PID
	int msgRec = 2;
	//numberRecieved storing variable
	int numR = 1;
	msgrcv(qid, (struct msgbuf *)&rcv, size, 257, 0);
	pid_t B_pid;
	pid_t A_pid;
	sscanf(rcv.greeting, "%d", &A_pid);
	msgrcv(qid, (struct msgbuf *)&rcv, size, 100, 0);
	sscanf(rcv.greeting, "%d", &B_pid);
	//used to keep track of pid for when messages exceed 10000
	
	while(rcv.greeting[0]!='T' || !(killed)){

		if(msgRec > 1000 && !(killed))
		{
			cout << "WILL TERMINATE: " << B_pid << endl;
			force_patch(B_pid);
			killed = true;
		}

		msgrcv(qid, (struct msgbuf *)&rcv, size, -300, 0);
			msgRec++;

		//sscanf(rcv.greeting, "%d", &numR);
		//message did not come from B
		if(rcv.m_type == 257)
			{
				cout << "Message recieved from PID: " << A_pid << endl;
				cout << "Number recieved: " << rcv.greeting;
				msgsnd(qid, (struct msgbuf *)&msg, size, 0);
			}

		else
			{
				cout << "Message recieved from PID: " << B_pid << endl;
				cout << "Number recieved: " << rcv.greeting << endl;
			}
		
	}

	msgctl(qid,IPC_RMID,NULL);
	return 0;
}