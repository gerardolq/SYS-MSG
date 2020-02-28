// Written by: Eunice Kang, Gerardo Lopez
// File Name: DataHub.cpp
// Description: Receives messages from ProbeA, ProbeB, and ProbeC
// 		Sends messages to ProbeA as a response to receiving a message from probeA
//		Terminates ProbeB if DataHub has received 10000 messages
//		DataHub terminates if it is no longer receiving messages from ProbeA, ProbeB, and ProbeC

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
	int qid = msgget(ftok(".",'u'),IPC_EXCL|IPC_CREAT|0600);	// create queue
	
	buf msg;
	msg.m_type = 20000;
	buf rcv;
	int size = sizeof(msg) - sizeof(long);
	
	bool killed = false; 	// shows if ProbeB has been killed or not
	int msgRec = 0; 	// number messages received
	int counter = 0; 	// counts the number of probes that have been terminated
	
	pid_t B_pid;
	pid_t A_pid;
	pid_t C_pid;
	bool aRec = 0, bRec = 0, cRec = 0; // set to false as we haven't received a message yet

	
	while(counter < 3){ // while all three Probes have not been terminated
		
		// if DataHub received at least 10000 messages AND
		// ProbeB has not been killed yet AND
		// ProbeB started to run
		if(msgRec >= 10000 && !(killed) && bRec) 
		{
			cout << "WILL TERMINATE: " << B_pid << endl;
			force_patch(B_pid); // terminate ProbeB
			killed = true;
			counter++;
		}

		msgrcv(qid, (struct msgbuf *)&rcv, size, -300, 0); // receive first message in queue
		msgRec++;	// incrememnt number of messages received

		if(rcv.m_type == 257) // if message is from ProbeA
		{
			if(!aRec){ // first time receiving a message from ProbeA (the pid)
				sscanf(rcv.greeting, "%d", &A_pid);
				aRec = true;
			}

			cout << "Message recieved from PID: " << A_pid << endl;
			cout << "Number recieved: " << rcv.greeting;
			msgsnd(qid, (struct msgbuf *)&msg, size, 0); // send acknowledgement to ProbeA
		}

		else if(rcv.m_type == 100) // message is from ProbeB
		{
			if(!bRec){ // first time receiving a message from ProbeB (the pid)
				sscanf(rcv.greeting, "%d", &B_pid);
				bRec = true;
			}

			cout << "Message recieved from PID: " << B_pid << endl;
			cout << "Number recieved: " << rcv.greeting << endl;
		}
		
		else //message is from ProbeC
		{ 
			if(!cRec){ // first time receiving a message from ProbeC (the pid)
				sscanf(rcv.greeting, "%d", &C_pid);
				cRec = true;
			}

			cout << "Message recieved from PID: " << C_pid << endl;
			cout << "Number recieved: " << rcv.greeting << endl;
		}

		if(rcv.greeting[0] == 'T') // if ProbeA sends message that it has been terminated
			counter++;

		if(rcv.m_type == 1) // if ProbeC sends message that it has been killed
			counter++;
		
	}

	msgctl(qid,IPC_RMID,NULL); // clear the queue
	
	// DONE :)
	return 0;
}
