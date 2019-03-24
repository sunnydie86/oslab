#include<stdio.h>
#include<stdlib.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<sys/types.h>
#include<sys/msg.h>
#include<signal.h>
#include<time.h>
#include<unistd.h>

void sighandler(int signal){
}

int main(){
	int k,m,f,s,i,j,p,q;
	int mid,sid;

	signal(SIGUSR1,sighandler);
	printf("enter no. of processes\n>");
	scanf("%d",&k);
	printf("enter maximum no. of pages\n>");
	scanf("%d",&m);
	printf("enter no. of frames\n>");
	scanf("%d",&k);

   	key_t key1 = ftok("SM1file",65); 
   	int SM1 = shmget(key1,4*k*m*sizeof(int),0666|IPC_CREAT); 
   	int *str = (int*)shmat(SM1,(void*)0,0);
   	for(i=0;i<2*k*m;i++){
   		if(i%4==3){
   			str[i] = 0;
   		}
   	}

   	char sm1[100];
   	sprintf(sm1,"%d",SM1);


   	key_t key2 = ftok("SM2file",65); 
   	int SM2 = shmget(key2,2*k*sizeof(int),0666|IPC_CREAT); 
   	str = (int*)shmat(SM2,(void*)0,0);
   	for(i=0;i<2*k;i++){
   		if(i%2==0){
   			str[i] = 0;
   		}
   	}

   	char sm2[100];
   	sprintf(sm2,"%d",SM2);

   	key_t key3 = ftok("MQ1file",65); 
   	int MQ1 = msgget(key3,0666|IPC_CREAT);

   	char mq1[100];
   	sprintf(mq1,"%d",MQ1);

  	key_t key4 = ftok("MQ2file",65); 
   	int MQ2 = msgget(key4,0666|IPC_CREAT);

   	char mq2[100];
   	sprintf(mq2,"%d",MQ2);


  	key_t key5 = ftok("MQ3file",65); 
   	int MQ3 = msgget(key5,0666|IPC_CREAT);
   	char mq3[100];
   	sprintf(mq3,"%d",MQ3);

   	//scheduler thread
   	if((sid=fork())==0){
   		char** args;
   		args = (char**)malloc(sizeof(char*)*3);
   		args[0] = "./SHED";
   		args[1] = mq1;
   		args[2] = mq2;
   		execvp(args[0],args);
   	}
 
   	char frame[100];
   	frame =  sprintf()

 	//MMU thread
   	if((mid=fork())==0){
   		char** args;
   		args = (char**)malloc(sizeof(char*)*4);
   		args[0] = "./MMU";
   		args[1] = mq2;
   		args[2] = mq3;
   		args[3] = sm1;
   		args[4] = sm2;   
   		args[5] = frame;		
   		execvp(args[0],args);
   	}
 
   	//process thread
   	for(i=0;i<k;i++){
   		srand(time(NULL));
   		p = (rand()%m)+1;
   		q = (rand()%(8*p))+(2*p);
   		int ref_string[q];
   		for(j=0;j<q;j++){
   			ref_string[j] = rand()%p+1;
   		}
  		char** args;
   		args = (char**)malloc(sizeof(char*)*4);
   		args[0] = "./PROCESS";
   		args[1] = (char*)ref_string;
   		args[2] = mq2;
   		args[3] = mq3;  		
   		execvp(args[0],args);   
   		sleep(0.25);		
   	}
   	pause();
   	kill(sid,SIGKILL);
   	kill(mid,SIGKILL);
   	return 0;	
}