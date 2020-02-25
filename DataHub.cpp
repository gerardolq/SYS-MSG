#include<iostream>
#include<time.h>
#include<cstdlib>
#include<unistd.h>
#include<sys/types.h>
#include<cstring>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

#include "force_patch.h"

using namespace std;

struct buf
{
	long m_type;
	char greeting[50];
};



int main()
{
	int counter = 0;

	int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);
	buf msg;
	msg.m_type = 20000;
	buf rcv;
	int size = sizeof(msg) - sizeof(long);
	bool killed = false;
	//starts at 0
	int msgRec = 0;
	//numberRecieved storing variable

	//msgrcv(qid, (struct msgbuf *)&rcv, size, 257, 0);
	pid_t B_pid;
	pid_t A_pid;
	pid_t C_pid;
	bool aRec = 0, bRec = 0, cRec = 0; // set to false as we haven't received a message yet
	//used to keep track of pid for when messages exceed 10000
	
	while(counter < 3){

		if(msgRec > 1000 && !(killed))
		{
			cout << "WILL TERMINATE: " << B_pid << endl;
			force_patch(B_pid);
			killed = true;
			counter++;
		}

		msgrcv(qid, (struct msgbuf *)&rcv, size, -300, 0);
			msgRec++;

		//sscanf(rcv.greeting, "%d", &numR);
		//message did not come from B
		if(rcv.m_type == 257)
		{
			if(!aRec)
				{sscanf(rcv.greeting, "%d", &A_pid);
				aRec = true;}

			cout << "Message recieved from PID: " << A_pid << endl;
			cout << "Number recieved: " << rcv.greeting;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0);
		}

		else if(rcv.m_type == 100)
		{
			if(!bRec)
				{sscanf(rcv.greeting, "%d", &B_pid);
				bRec = true;}

			cout << "Message recieved from PID: " << B_pid << endl;
			cout << "Number recieved: " << rcv.greeting << endl;
		}
		else{
			if(!cRec)
				{sscanf(rcv.greeting, "%d", &C_pid);
				cRec = true;}

			cout << "Message recieved from PID: " << C_pid << endl;
			cout << "Number recieved: " << rcv.greeting << endl;
		}

		if(rcv.greeting[0] == 'T')
			counter++;

		if(rcv.m_type == 1)
			{counter++;}
		
	}

	msgctl(qid,IPC_RMID,NULL);
	return 0;
}