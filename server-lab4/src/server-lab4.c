/* File name: server-lab4.c
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
FILE * fp;
/*
 * Function Name: error
 * Purpose: Print error message
 * param: char *
 * version: 1.0
 * author: SeyedAmin SeyedMahmoudian,Flaming Patel
 * return: void
 */
void error(char * msg){
	perror(msg);
	exit(EXIT_FAILURE);
}
/*
 * Function Name: main
 * Purpose: Run the server , print the PID, open the file,
 * 			and write the information transmitted from the client to the file.
 * param: None
 * version:1
 * author: SeyedAmin SeyedMahmoudian, Flaming Patel
 * return: int
 */
int main(void) {

	printf("PID: %d : Running....\n",getpid());//print the PID
	int chid=0;
	if((chid=ChannelCreate(0))==-1){//create a channel to listen to incoming msg
		error("server: ERROR: Couldn't create a channel\n");
	}
	clientMsg client;
	serverMsg server;
	int rcvId=0;
	fflush(stdout);

	while(1){
		fp=fopen("/home/qnxuser/logs.txt","ab+");//open or create the file for log
		memset(server.msg,0,256);//override he array with 0 to remove junks
		if((rcvId=MsgReceive(chid,&client,sizeof(client),NULL))==-1){//Receive a message from the client
			error("Problem receiving the message from the client.\n");
		}

		if(client.option != 2){//check what client wants to do if it is 1 continue other was terminate the client
			fprintf(fp,"\nClient %d entered\n",client.pid);//write client pid to the file
			printf("\nClient %d entered\n",client.pid);
			server.clientPid=client.pid;
		}else{
			fprintf(fp,"Client %d left\n",client.pid);//write client pid to the file
			printf("Client %d left\n",client.pid);
			fclose(fp);//close the file
		}
		fprintf(fp,"Client sent: %s",client.msg);//write client msg to the file
		printf("Client sent: %s",client.msg);//write client msg to the file
		fprintf(fp,"Client option: %c\n",client.option);//write client option to the file
		printf("Client option: %c\n",client.option);//write client option to the file
		initReply(&client,&server);//invoke helper function

		int status=MsgReply(rcvId,1,&server,sizeof(server));//reply to the client
		if(status==-1){//in case reply was not successfull print the error
			error("Problem while replying the message to client.\n");
		}
		fclose(fp);//close the file
	}
	return EXIT_SUCCESS;
}
/*
 * Function Name: initReply
 * Purpose: Determine what client likes to do , if 1 is being sent found the value in the index request,
 * 			if 2 was entered count the number of the letter requested
 * param: clientMsg*,serverMsg*
 * version:1
 * author: SeyedAmin SeyedMahmoudian,Flaming Patel
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
			fprintf(fp,"Server reply: %s\n",server->msg);//save the reply to the file
			printf("Server reply: %s\n",server->msg);
			free(newString);
		}
	}else if(client->optionFlag=='2'){
		int i = 0;
		int count = 0;
		char chCount;
		while(client->msg[i]!='\0'){//get the count of how many char is in the string
			if(client->msg[i]==client->option){//count how many times requested char is in the string
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
		fprintf(fp,"Server reply: %s\n",server->msg);//save the msg to the file
		printf("Server reply: %s\n",server->msg);//save the msg to the file
		free(newString);//free the memory made for new string
	}
}

