/* File name: struct.h  - client
 * Purpose:header file used in assignment
 *  CST8244 - Real-Time Programming
 *  Version: 1
 *  Author: Flaming Patel ,  Seyedamin Seyedmahmoudian
 *  Date: 08 March 2018
 */
#ifndef SRC_STRUCT_H_
#define SRC_STRUCT_H_

typedef struct sendByClient{//msg send by client
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

#endif /* SRC_STRUCT_H_ */
