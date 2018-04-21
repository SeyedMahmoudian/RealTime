/* File name: struct.h  - client
 * Purpose:header file used in assignment
 *  CST8244 - Real-Time Programming
 *  Version: 6
 *  Author: Seyedamin Seyedmahmoudian
 *  Date: 2018-04-20
 */
#ifndef SRC_STRUCT_H_
#define SRC_STRUCT_H_

typedef struct sendByClient{//msg send by client
	struct _pulse pulse;
	char option;//option set by user
	char msg[256];//msg send by user
	int digitOption;
	char optionFlag;
	int pid;
	char choice;
}clientMsg;
typedef struct sendByServer{//reply from the server
	int clientPid;
	char  msg[256];//msg send by server
	int regStat;
}serverMsg;

typedef union{
	struct _pulse pulse;
	char myMsg[256];
}my_message_t;
#endif /* SRC_STRUCT_H_ */
