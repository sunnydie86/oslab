#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include<stdlib.h>


struct queue{ 
	long mtype;
	int pid;
}ready;

struct msgbuf {
    long mtype;        
    int ref;
    int pid;     
}message;

struct msgrcv {
    long mtype;        
    int frame;    
}reply;

void sighandler_1(int signal){
	if(signal==SIGUSR1){
		pause();
	}
	return;
}

void sighandler_2(int signal){
	if(signal==SIGUSR2){

	}
	return;
}


int main(int argc,char* argv[]){


	int  i = 0,j;
	int MQ1 = atoi(argv[2]);
	int MQ2 = atoi(argv[3]);
	int m  = atoi(argv[4]);
	int* ref_string = (int*)argv[1];
	signal(SIGUSR1,sighandler_1);
	signal(SIGUSR2,sighandler_2);

	ready.pid = getpid();
	ready.mtype = 1;
	msgsnd(MQ1,&ready,sizeof(ready),0);
	pause();
	while(i<m){
		message.ref = ref_string[i];
		message.mtype = 1;
		msgsnd(MQ2,&message,sizeof(message),0);
		msgrcv(MQ2,&reply,sizeof(reply),2,0);
		if(reply.frame==-2){
			i--;
			pause();
		}
		i++;
	}
	message.ref = -9;
	message.mtype = 1;
	msgsnd(MQ2,&message,sizeof(message),0);	
	return 0;
}