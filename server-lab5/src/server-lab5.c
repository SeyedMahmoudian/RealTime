/* File name: server-lab5.c
 * Purpose:Server side of the program that will run in background and
 * 		  acknowledge the when a message is received,and perform required action base on the option it was given
 * 		  it require a port number
 *  CST8244 - Real-Time Programming
 *  Version: 1
 *  Author:   Seyedamin Seyedmahmoudian
 *  Date: 08 March 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <string.h>
#include <sys/neutrino.h>
#include <limits.h>
#include "struct.h"
void initReply(clientMsg*,serverMsg*);
FILE * log;
int clientCounter=0;
int semaphoreCounter=3;
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
	log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
	fprintf(log,msg);
	fclose(log);
	exit(EXIT_FAILURE);
}
void logme(char * msg){
	printf("%s\n",msg);
	log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
	fprintf(log,msg);
	fclose(log);
}
/*
 * Function Name: main
 * Purpose: Run the server , print the PID, open the file,
 * 			and write the information transmitted from the client to the file.
 * param: None
 * version:1
 * author: SeyedAmin SeyedMahmoudian, 
 * return: int
 */
int main(void) {
	int chid=0;
	if((chid=ChannelCreate(0))==-1){//create a channel to listen to incoming msg
		error("server: ERROR: Couldn't create a channel\n");
	}
	printf("\nPID: %d : Running on Channel : %d\n",getpid(),chid);//print the PID
	clientMsg client;
	serverMsg server;
	int rcvId=0;
	fflush(stdout);
	int semaphores[10][2]={0};

	while(1){
		memset(server.msg,0,256);//override he array with 0 to remove junks
		if((rcvId=MsgReceive(chid,&client,sizeof(client),NULL))==-1){//Receive a message from the client
			error("\nProblem receiving the message from the client.\n");
		}
		int isExistInTable = 0; //false;
		int tableSize=sizeof(semaphores)/sizeof(semaphores[0]);
		for(int i = 0; i < tableSize; i++){//check to see if client exist
			if(semaphores[i][0] == client.pid){
				isExistInTable = 1; //client exist in table
				break;
			}
		}
		if(isExistInTable == 0){//if client does not exist in table
			//add the client to the table
			for(int i=0;i<tableSize;i++){
				//find the next available space
				if(semaphores[i][0]==0){
					semaphores[i][0]=client.pid;
					break;
				}
			}
		}
		char option=client.option;
		int i=0;
		switch(option){
		case 1://client need to register
			if(semaphoreCounter==0){//too many client already registered
				char * msg="\nTo many client connected\n";
				logme(msg);
				strcpy(server.msg,msg);
				client.option=0;
				semaphores[clientCounter][0]=client.pid;//save the client pid to the table
				semaphores[clientCounter][1]=0;//set the option to zero
			}else{//there are space for more client to register
				int size=sizeof(semaphores)/sizeof(semaphores[0]);
				int i=0;
				for(i=0;i<size;i++){
					if(semaphores[i][0]==client.pid){//find the client
						if(semaphores[i][1]==0){//client was foind
							semaphores[i][1]=1;//change the option to one for register
							break;
						}
					}
				}
				char *msg ="\nYou have been successfully registered with server";
				strcpy(server.msg,msg);
				logme(server.msg);
				semaphoreCounter--;
				clientCounter++;
			}
			break;
		case 2://client wants to un register
			if(semaphoreCounter==3){
				char *msg="\nNo one is registered";
				strcpy(server.msg,msg);
				logme(server.msg);
			}
			int size=sizeof(semaphores)/sizeof(semaphores[0]);

			int flag=0;
			for(i=0;i<size;i++){
				if(semaphores[i][0]==client.pid){//client was found
					flag=0;
					if(semaphores[i][1]==1){//is client register or not
						semaphores[i][1]=0;//de register the client
						semaphoreCounter++;//increment the counter
						char *msg="\nYou have been successfully Unregistered with server";
						strcpy(server.msg,msg);
						logme(server.msg);
						log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
						printf("Client PID : %d",client.pid);
						fprintf(log,"Client PID : %d",client.pid);
						fclose(log);
						break;
					}//end of if to check if client is registered or not
					else{//client was not register
						char * msg="\nYou are not registered!";
						strcpy(server.msg,msg);
						logme(server.msg);
						log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
						fprintf(log,"Client PID : %d",client.pid);
						fclose(log);
						flag=1;

					}
				}//end of check to see client exist or not
				else{
					flag=1;
				}
			}//end of for loop
			if(flag){
				char * msg="\n No Such client exist!";
				strcpy(server.msg,msg);
				logme(server.msg);
				log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
				fprintf(log,"client pid:");
				fclose(log);
			}
			break;
		case 3:
			//BUG when client send the option buffer is messed up
			size=sizeof(semaphores)/sizeof(semaphores[0]);
			for(i=0;i<size;i++){
				if(semaphores[i][0]==client.pid){//find the client
					if(semaphores[i][1]==1){//check if it is registered
						flag=0;//client was found ant it is reged
						log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
						fprintf(log,"\nClient %d informations are as follow:",client.pid);
						printf("\nClient %d information as follow:",client.pid);
						fprintf(log,"\nClient sent: %s",client.msg);//write client msg to the file
						printf("\nClient sent: %s",client.msg);//write client msg to the file
						fprintf(log,"\nClient choice: %c",client.choice);//write client option to the file
						printf("\nClient choice: %c\n",client.choice);//write client option to the file
						fclose(log);
						initReply(&client,&server);
						break;
					}else{
						flag=0;//client was found but not reged
						char *msg="\nYou need to register with the server first!Please press 1";
						strcpy(server.msg,msg);
						logme(server.msg);
						log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
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
			if(flag){
				char *msg="\nPlease register first ";
				strcpy(server.msg,msg);
				logme(server.msg);
				log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
				fprintf(log,"PID: %d\n",client.pid);
				fclose(log);
			}

			break;
		case 4:
			log=fopen("/home/qnxuser/lab5-logs.txt","ab+");
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
		}//end of switch

		int status=MsgReply(rcvId,1,&server,sizeof(server));//reply to the client
		if(status==-1){//in case reply was not successfull print the error
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
 * author: SeyedAmin SeyedMahmoudian,
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

