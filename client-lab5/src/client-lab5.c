/* File name: client.c
 * Purpose: Client side of the program that will send a message to server
 * 			it requires hostname and port number of the server
 *  CST8244 - Real-Time Programming
 *  Version: 1
 *  Author:  Flaming Patel , Seyedamin Seyedmahmoudian
 *  Date: 08 March 2018
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <ctype.h>
#include "struct.h"
/***************************************
 * Constant
 ***************************************/
#define TRUE 1
#define FALSE 0
int flag;
/***************************************
 * Function Prototype
 ***************************************/
void check(int,clientMsg,serverMsg);//helper function
void validate(char*,clientMsg*);
/*
 * Function Name: error
 * Purpose: Print error message
 * param: char *
 * version: 1.0
 * author: SeyedAmin SeyedMahmoudian,Flaming Patel
 * return: void
 */
void error(char*msg){
	perror(msg);
	exit(EXIT_FAILURE);
}
/*
 * Function Name: main
 * Purpose:
 * param: int , char *
 * version:1
 * author: SeyedAmin SeyedMahmoudian,Flaming Patel
 * return: int
 */
int main(int argc, char *argv[]) {

	if (argc < 2) {
		fprintf(stderr, "usage ./client-lab4 PID \n", argv[0]);
		return EXIT_FAILURE;
	}
	pid_t serverPid = atoi(argv[1]); //server pid will be second arg
	/*establish a connection between the calling process and
	 *the channel specified by chid owned by the process specified by pid on the node specified by nd.
	 */
	int coid = ConnectAttach(ND_LOCAL_NODE, serverPid, 1, _NTO_SIDE_CHANNEL, 0);
	if (coid == -1) {
		error("Connection to server failed.\n");
	}
	clientMsg client;
	serverMsg server;
	int terminate = FALSE;
	while(terminate!=TRUE){
		int option=0;
		printf("1)Register with server.\n2)Unregister with server\n"
				"3)To send a message to server.\n4)Quit\n");
		fflush(stdout);
		scanf("%d",&option);//reading user input for option

		while ((getchar()) != '\n'); // clearing buffer
		switch (option) {
		case 1:
			client.option=1;
			client.pid=getpid();
			check(coid,client,server);
		break;
		case 2:
			client.option=2;
			client.pid=getpid();
			check(coid,client,server);
			break;
		case 3:

			printf("Enter a message: ");
			fflush(stdout);
			fgets(client.msg,256,stdin);//copy user message from standard input to client message char
			client.pid=getpid();//get client pid
			do{
				printf("Please enter a integer value or character: ");
				flag=TRUE;
				char input[256];
				fflush(stdout);
				fgets(input,256,stdin);
				validate(input,&client);//helper method explain in function header
			}while(flag==TRUE);
			client.option=3;
			check(coid,client,server);//helper method explain in function header
			client.optionFlag=0;
			break;
		case 4:
			client.option=4;//set the option to 4
			check(coid,client,server);//invoke helper method
			ConnectDetach(coid);//disconnect from server
			terminate=TRUE;//stop the program
			break;
		default:
			printf("Wrong option was chosen\n");
			break;
		}


	}
	return EXIT_SUCCESS;

}

/*
 * Function Name: check
 * Purpose: Helper function to send and recieve messages from or to the server
 * param: int,clientMsg,serverMsg
 * version:1
 * author: SeyedAmin SeyedMahmoudian,Flaming Patel
 * return: -
 */
void check(int coid,clientMsg client,serverMsg server){

	int status=MsgSend(coid,&client,sizeof(client),&server,sizeof(server));//send the message to sever
	if(status==-1){//if status is -1 the message failed
		error("Problem while sending the message to server.\n");
	}

	memset(client.msg,0,256);//fill msg array with zero to override junk
	if(strlen(server.msg)==0){
		strcpy(server.msg,"Bye");//display bye when client enter 2
	}
	printf("Message from server : %s\n",server.msg);//display the string which it was sent by server

}
/*
 * Function Name: validate
 * Purpose: will determine if the user has entered a digit or a message
 * 			if the user entered a digit it will treat the digit set the flag
 * 			if the user entered char it will set the char flag
 * param: char *, clientMsg *
 * version:1
 * author: SeyedAmin SeyedMahmoudian,Flaming Patel
 * return: -
 */
void validate(char* input,clientMsg* msg){
	msg->pid=getpid();
	if(strlen(input)>2){
		/* Check for the input length greater than one means it is a string
		 * or a double digit was entered
		 * */
		//msg->option=input[0];//check the first char in the input
		msg->choice=input[0];
		if(isdigit(msg->choice)){ //if it is a digit if yes it means double digit was entered
			size_t msgLength = strlen(input);
			int isAlpha=FALSE;
			int i=0;
			int count=0;
			while(input[i] !='\0'){//loop until end of line
				if(isalpha(input[i])){//if the first char is alpha set the flag
					isAlpha=TRUE;
				}
				i++;
			}
			if(!isAlpha){//a double digit was entered set flag to one
				msg->optionFlag='1';
				msg->digitOption=atoi(input);
				flag=FALSE;
			}
		}else if(isalpha(msg->choice)){//a string was entered set the flag to 2
			msg->optionFlag='2';
			flag=FALSE;
		}
	}
	else{//single digit was entered or single char
		//msg->option=input[0];
		msg->choice=input[0];
		if(isdigit(msg->choice)){//single digit was entered
			msg->optionFlag='1';
			msg->digitOption=atoi(input);
			flag=FALSE;
		}else if(isalpha(msg->choice)){//single char was entered
			msg->optionFlag='2';
			flag=FALSE;
		}
	}

}

