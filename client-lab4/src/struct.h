
#ifndef SRC_STRUCT_H_
#define SRC_STRUCT_H_

typedef struct sendByClient{//msg send by client
	char option;//option set by user
	char msg[256];//msg send by user
	int digitOption;
	char optionFlag;
	int pid;
}clientMsg;
typedef struct sendByServer{//reply from the server
	int clientPid;
	char  msg[256];//msg send by server
}serverMsg;

#endif /* SRC_STRUCT_H_ */
