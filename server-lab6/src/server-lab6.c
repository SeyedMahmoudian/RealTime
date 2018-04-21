/* File name: server-lab6.c
 * Purpose:Server side of the program that will run in background and
 * 		  acknowledge the when a message is received,and perform required action base on the option it was given
 * 		  it require a port number
 *  CST8244 - Real-Time Programming
 *  Version: 6
 *  Author: Seyedamin Seyedmahmoudian
 *  Date: 2018-04-20
 */
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include "struct.h"

int getIndexofClient(int ,int );
void initReply(clientMsg*,serverMsg*);
void startTimer(int, int ,struct sigevent,struct itimerspec ,timer_t );

FILE * log;
int clientCounter=0;
int semaphoreCounter=3;
int semaphores[10][2]={0};
int timer_table[10]={0};
/*
 * Function Name: error
 * Purpose: Print error message
 * param: char *
 * version: 1.0
 * author: SeyedAmin SeyedMahmoudian
 * return: void
 */
void error(char * msg){
	perror(msg);
	log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
	fprintf(log,msg);
	fclose(log);
	exit(EXIT_FAILURE);
}
void logme(char * msg){
	printf("%s\n",msg);
	log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
	fprintf(log,msg);
	fclose(log);
}
/*
 * Function Name: main
 * Purpose: Run the server , print the PID, open the file,
 * 			and write the information transmitted from the client to the file.
 * param: None
 * version:1
 * author: SeyedAmin SeyedMahmoudian
 * return: int
 */
int main(void) {
	int chid=0;
	int flag;
	int size;
	if((chid=ChannelCreate(0))==-1){//create a channel to listen to incoming msg
		error("server: ERROR: Couldn't create a channel\n");
	}
	printf("\nPID: %d : Running on Channel : %d\n",getpid(),chid);//print the PID

	int rcvId;
	fflush(stdout);
	clientMsg client;
	serverMsg server;

	struct sigevent event;
	struct itimerspec itime;
	timer_t timer_id;


	while(1){
		memset(server.msg,0,256);//override he array with 0 to remove junks
		rcvId=MsgReceive(chid,&client,sizeof(client),NULL);//Receive a message from the client
		if(rcvId==-1){
			error("\nProblem receiving the message from the client.\n");
		}

		int isExistInTable = 0; //false;
		int tableSize=sizeof(semaphores)/sizeof(semaphores[0]);
		for(int i = 0; i < tableSize; i++){
			if(semaphores[i][0] == client.pid){
				isExistInTable = 1; //true
				break;
			}
		}
		if(isExistInTable == 0){//if client does not exist in table
			for(int i=0;i<tableSize;i++){
				//find the next available space
				if(semaphores[i][0]==0){
					semaphores[i][0]=client.pid;
					break;
				}
			}
		}
		if(rcvId==0){
			int clientPid = semaphores[client.pulse.code][0];
			printf("\nPulse was received for %d\n",clientPid);
			log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
			fprintf(log,"Pulse was received for %d",clientPid);
			fclose(log);
			semaphores[client.pulse.code][1]=0;//unregister the client
			semaphoreCounter++;//increment semaphore counter
			continue;
		}
		char option=client.option;
		int i=0;
		switch(option){
		case 1://client like to register
			if(getIndexofClient(client.pid,tableSize)==-1){
				error("could not get the index of timer line 118 case 1");
			}
			if(semaphoreCounter==0){
				/*
				 * no space left to register already have 3 registered client
				 */
				char * msg="\nTo many client connected\n";
				logme(msg);
				strcpy(server.msg,msg);
				client.option=0;
				semaphores[clientCounter][0]=client.pid;
				semaphores[clientCounter][1]=0;
			}else{
				/*
				 * there are space left to register one new client
				 */
				size=sizeof(semaphores)/sizeof(semaphores[0]);
				int i=0;
				for(i=0;i<size;i++){
					if(semaphores[i][0]==client.pid){
						if(semaphores[i][1]==0){
							semaphores[i][1]=1;
							break;
						}
					}
				}
				/*
				 * send reg ok message to the client
				 */
				char *msg ="\nYou have been successfully registered with server";
				strcpy(server.msg,msg);
				logme(server.msg);
				semaphoreCounter--;
				clientCounter++;
				/*
				 * start the timer to 15 second by calling startTimer function
				 */
				startTimer(chid,getIndexofClient(client.pid,tableSize),event,itime,timer_id);
			}
			break;
		case 2:
			/*
			 * client likes to de-register
			 */
			if(semaphoreCounter==3){//check to see if anyone is registered if not do not waste time
				char *msg="\nNo one is registered";
				strcpy(server.msg,msg);
				logme(server.msg);
			}else{//there some client registered in the server
				size=sizeof(semaphores)/sizeof(semaphores[0]);
				int flag=0;
				for(i=0;i<size;i++){
					if(semaphores[i][0]==client.pid){//client was found
						flag=0;
						if(semaphores[i][1]==1){//un registered the client now
							semaphores[i][1]=0;
							semaphoreCounter++;
							char *msg="\nYou have been successfully Unregistered with server";
							strcpy(server.msg,msg);
							logme(server.msg);
							log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
							printf("Client PID : %d",client.pid);
							fprintf(log,"Client PID : %d",client.pid);
							fclose(log);
							timer_delete(getIndexofClient(client.pid,tableSize));//delete the timer
							break;
						}//end of if to check if client is registered or not
						else{//client was not registered send the message saying so
							char * msg="\nYou are not registered!";
							strcpy(server.msg,msg);
							logme(server.msg);
							log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
							fprintf(log,"Client PID : %d",client.pid);
							fclose(log);
							flag=1;

						}
					}//end of check to see client exist or not
					else{
						flag=1;
					}
				}//end of for loop
				if(flag){//there was no client in the table
					char * msg="\n No Such client exist!";
					strcpy(server.msg,msg);
					logme(server.msg);
					log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
					fprintf(log,"client pid:");
					fclose(log);
				}
			}
			break;
		case 3://start of message passing
			//before this option set we have hidden option 5 that will destroy the timer
			size=sizeof(semaphores)/sizeof(semaphores[0]);
			for(i=0;i<size;i++){
				if(semaphores[i][0]==client.pid){//find the client
					if(semaphores[i][1]==1){//check if it is registered
						flag=0;//client was found and it is reged
						log=fopen("/home/qnxuser/lab6-logs.txt","ab+");//open the file to write
						fprintf(log,"\nClient %d informations are as follow:",client.pid);
						printf("\nClient %d information as follow:",client.pid);
						fprintf(log,"\nClient sent: %s",client.msg);//write client msg to the file
						printf("\nClient sent: %s",client.msg);//write client msg to the file
						fprintf(log,"\nClient choice: %c\n",client.choice);//write client option to the file
						printf("\nClient choice: %c\n",client.choice);//write client option to the file
						fclose(log);
						initReply(&client,&server);
						break;
					}else{
						flag=0;//client was found but not reged
						char *msg="\nYou need to register with the server first!Please press 1";
						strcpy(server.msg,msg);
						logme(server.msg);
						log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
						fprintf(log,"\nUnregistered PID: %d",client.pid);
						fclose(log);
						break;
					}

				}//end of find client
				else{
					flag=1;
					break;
				}
			}//end of for loop
			if(flag){//client is not registered
				char *msg="\nPlease register first ";
				strcpy(server.msg,msg);
				logme(server.msg);
				log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
				fprintf(log,"PID: %d\n",client.pid);
				fclose(log);
			}
			startTimer(chid,getIndexofClient(client.pid,tableSize),event,itime,timer_id);//start the timer again
			break;
		case 4:
			timer_delete(getIndexofClient(client.pid,tableSize));//delete the timer because the client is leaving
			log=fopen("/home/qnxuser/lab6-logs.txt","ab+");
			fprintf(log,"\nClient %d left\n",client.pid);//write client pid to the file
			fclose(log);//close the log
			printf("\nClient %d left\n",client.pid);
			size=sizeof(semaphores)/sizeof(semaphores[0]);
			i=0;
			for(i=0;i<size;i++){
				if(semaphores[i][0]==client.pid){
					int c=0;
					for(c=i;c<size-1;c++){
						semaphores[c][0]=semaphores[c+1][0];//remove the client
					}
				}//end of if
			}//end of first for
			clientCounter--;
			break;

		case 5:
			// hidden case to delete timer in case of sending a message this will be set by client
			timer_delete(getIndexofClient(client.pid,tableSize));//delet the timer because client is working with the server
			memset(server.msg,'\0',256);//empty the buffer
			break;
		}//end of switch

		int status=MsgReply(rcvId,1,&server,sizeof(server));//reply to the client
		if(status==-1){//in case reply was not successful print the error
			error("Problem while replying the message to client.\n");
		}

	}//end of loop
	return EXIT_SUCCESS;
}//end of main



/*
 * Function Name: initReply
 * Purpose: Determine what client likes to do , if 1 is being sent found the value in the index request,
 * 			if 2 was entered count the number of the letter requested
 * param: clientMsg*,serverMsg*
 * version:1
 * author: SeyedAmin SeyedMahmoudian
 * return:-
 */
void initReply(clientMsg* client, serverMsg* server){

	if(client->optionFlag=='1'){
		size_t length=strlen(client->msg);//find the size of the msg
		length=length-1;//index start 0
		int selected = client->digitOption;

		if((selected>length)||(selected<0)){//check to see correct index is provided
			char * temp="Please choose the appropriate index...\n";
			strcpy(server->msg,temp);
		}
		else{
			char foundChar=client->msg[selected];//find the char base on the index provided
			char *msg="Server found: ";
			size_t length=strlen(msg);//get the length of the msg for reply
			char *newString=(char*)malloc(sizeof(length+2));//allocate memory for new string for concatenation
			strcpy(newString,msg);//copy the server msg to the memory
			newString[length++]=foundChar;//concatinate the founded char
			newString[length]='\0';//finish the char with end of the line
			strcpy(server->msg,newString);//finally copy the full char from memory to the struct
			logme(server->msg);
			printf("Server reply: %s\n",server->msg);
			free(newString);
		}
	}else if(client->optionFlag=='2'){
		int i = 0;
		int count = 0;
		char chCount;
		while(client->msg[i]!='\0'){//get the count of how many char is in the string
			if(client->msg[i]==client->choice){//count how many times requested char is in the string
				count++;
			}
			i++;
		}
		chCount=count+'0';
		char *msg="Server found:- ";
		size_t length=strlen(msg);//get the length of the msg for reply
		char *newString=(char*)malloc(sizeof(length+20));//allocate memory for new string for concatenation
		strcpy(newString,msg);//copy the server msg to the memory
		newString[length++]=chCount;//add the count the msg
		newString[length]='\0';//terminate the line
		strcat(newString," times letter used\n");
		strcpy(server->msg,newString);//copy the new string to struct for transfer
		logme(server->msg);
		printf("Server reply: %s\n",server->msg);//save the msg to the file
		free(newString);//free the memory made for new string
	}
}
/*
 * Function Name: getIndexofClient
 * Purpose: Get the index of the client from the semaphore table
 * param: int clientPid, int tableSize
 * version:1
 * author: SeyedAmin SeyedMahmoudian
 * return:client index or -1 if not exist
 */
int getIndexofClient(int clientpid,int tableSize){
	for(int i = 0; i < tableSize; i++){
		if(semaphores[i][0] == clientpid){
			return i;
		}
	}
	return -1;
}
/*
 * Function Name:startTimer
 * Purpose: Start the timer for the sepcefic client
 * param: int chid,int index, struct sigevent , struct itimespec, timer_t
 * version:1
 * author: SeyedAmin SeyedMahmoudian
 * return: -
 */
void startTimer(int chid,int index,struct sigevent event,struct itimerspec itime,timer_t timer_id){
	event.sigev_notify = SIGEV_PULSE;//set the flag
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0,chid,_NTO_SIDE_CHANNEL, 0);//get the channel of the timer to structure
	event.sigev_code = index;//save the client index into structure
	timer_create(CLOCK_REALTIME, &event, &timer_id);//create the timer
	itime.it_value.tv_sec = 15;//set the interval to 15 second
	timer_settime(timer_id, 0, &itime, NULL);//start the timer
	timer_table[index]=timer_id;//save the timer id into the table

}
/*
 * Function Name:deleteTimer
 * Purpose: Delete the timer for specific client
 * param: int index
 * version:1
 * author: SeyedAmin SeyedMahmoudian
 * return: -
 */
void deletTimer(int index){
	timer_delete(index);
}
