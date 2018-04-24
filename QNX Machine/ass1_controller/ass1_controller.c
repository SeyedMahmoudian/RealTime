#include <stdlib.h>
#include <stdio.h>
#include <proj.h>
#include <string.h>

void* ScanId();
/*
 * ENTERING
 */
void* UnlockLeftDoor_LS();
void* OpenLeftDoor_LS();
void* TakeWeight_LS();
void* CloseLeftDoor_LS();
void* LockLeftDoor_LS();
void* UnlockRightDoor_LS();
void* OpenRightDoor_LS();
void* CloseRightDoor_LS();
void* LockRightDoor_LS();

/*
 * EXITING
 */
void* UnlockLeftDoor_RS();
void* OpenLeftDoor_RS();
void* TakeWeight_RS();
void* CloseLeftDoor_RS();
void* LockLeftDoor_RS();
void* UnlockRightDoor_RS();
void* OpenRightDoor_RS();
void* CloseRightDoor_RS();
void* LockRightDoor_RS();

char* outMsg = "";
char* msgReceivedFromInputs[256];
typedef void *(*StateFunc)();
int rcvidForInputs, chidInput;
int replyToInputs = 0;
int replyFromDisplay;
int connectionIdToDisplay;

int main(int argc, char *argv[]) {

	StateFunc stateFunc = ScanId;

	int weight;

	printf("PID : %d : Running...\n", getpid());

	chidInput = ChannelCreate(0);

	connectionIdToDisplay = ConnectAttach(0, atoi(argv[1]), 1, 0, 0);

	if (connectionIdToDisplay == -1) {
		printf("Cannot Connect to Server. Please try again\n");
		return EXIT_FAILURE;
	}

	while (1) {

		stateFunc = (StateFunc) (*stateFunc)();
	}
	return EXIT_SUCCESS;
}

/*
 * ENTERING
 */
void *ScanId() {
	char* commandMsg;
	char msg[256];

	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	commandMsg = strtok(msgReceivedFromInputs, " ");

	if (strcmp(commandMsg, inMessage[LEFT_SCAN]) == 0) {
		strncpy(msg, outMessage[LEFT_SCAN_COMPLETE], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return UnlockLeftDoor_LS;

	} else if (strcmp(commandMsg, inMessage[RIGHT_SCAN]) == 0) {
		strncpy(msg, outMessage[RIGHT_SCAN_COMPLETE], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return UnlockRightDoor_RS();
	}
	return ScanId;
}

void *UnlockLeftDoor_LS() {
	char msg[256];

	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));

	if (strcmp(msgReceivedFromInputs, inMessage[GUARD_LEFT_UNLOCK]) == 0) {
		strncpy(msg, outMessage[LEFT_DOOR_UNLOCKED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return OpenLeftDoor_LS;
	}
	return UnlockLeftDoor_LS;
}

void *OpenLeftDoor_LS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}

	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[LEFT_OPEN]) == 0) {
		strncpy(msg, outMessage[LEFT_DOOR_OPEN], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return TakeWeight_LS;
	}
	return OpenLeftDoor_LS;
}
void* TakeWeight_LS() {
	char* commandMsg;
	char msg[256];

	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}

	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));

	commandMsg = strtok(msgReceivedFromInputs, " ");
	if (strcmp(commandMsg, inMessage[WEIGHT_SCALE]) == 0) {
		strncpy(msg, outMessage[WEIGHT_SCANED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return CloseLeftDoor_LS;
	}
	return TakeWeight_LS;
}
void* CloseLeftDoor_LS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}

	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));

	if (strcmp(msgReceivedFromInputs, inMessage[LEFT_CLOSED]) == 0) {
		strncpy(msg, outMessage[LEFT_DOOR_CLOSED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return LockLeftDoor_LS;
	}
	return CloseLeftDoor_LS;
}
void* LockLeftDoor_LS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}

	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[GAURD_LEFT_LOCK]) == 0) {
		strncpy(msg, outMessage[LEFT_DOOR_LOCKED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return UnlockRightDoor_LS;
	}
	return LockLeftDoor_LS;
}
void* UnlockRightDoor_LS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[GAURD_RIGHT_UNLOCK]) == 0) {
		strncpy(msg, outMessage[RIGHT_DOOR_UNLOCKED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return OpenRightDoor_LS;
	}
	return UnlockRightDoor_LS;
}

void* OpenRightDoor_LS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[RIGHT_OPEN]) == 0) {
		strncpy(msg, outMessage[RIGHT_DOOR_OPEN], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return CloseRightDoor_LS;
	}
	return OpenRightDoor_LS;
}
void* CloseRightDoor_LS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}

	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[RIGHT_CLOSED]) == 0) {
		strncpy(msg, outMessage[RIGHT_DOOR_CLOSED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return LockRightDoor_LS;
	}
	return CloseRightDoor_LS;
}
void* LockRightDoor_LS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[GAURD_RIGHT_LOCK]) == 0) {
		strncpy(msg, outMessage[RIGHT_DOOR_LOCKED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return ScanId;
	}
	return LockRightDoor_LS;
}

/*
 * EXITING
 */
void* UnlockRightDoor_RS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[GAURD_RIGHT_UNLOCK]) == 0) {
		strncpy(msg, outMessage[RIGHT_DOOR_UNLOCKED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return OpenRightDoor_RS;
	}
	return UnlockRightDoor_RS;
}

void* OpenRightDoor_RS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[RIGHT_OPEN]) == 0) {
		strncpy(msg, outMessage[RIGHT_DOOR_OPEN], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return TakeWeight_RS;
	}
	return OpenRightDoor_RS;
}
void* TakeWeight_RS() {
	char* commandMsg;
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	commandMsg = strtok(msgReceivedFromInputs, " ");
	if (strcmp(commandMsg, inMessage[WEIGHT_SCALE]) == 0) {
		strncpy(msg, outMessage[WEIGHT_SCANED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return CloseRightDoor_RS();
	}
	return TakeWeight_RS;
}

void* CloseRightDoor_RS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[RIGHT_CLOSED]) == 0) {
		strncpy(msg, outMessage[RIGHT_DOOR_CLOSED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return LockRightDoor_RS;
	}
	return CloseRightDoor_RS;
}
void* LockRightDoor_RS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[GAURD_RIGHT_LOCK]) == 0) {
		strncpy(msg, outMessage[RIGHT_DOOR_LOCKED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return UnlockLeftDoor_RS;
	}
	return LockRightDoor_RS;
}

void *UnlockLeftDoor_RS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[GUARD_LEFT_UNLOCK]) == 0) {
		strncpy(msg, outMessage[LEFT_DOOR_UNLOCKED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return OpenLeftDoor_RS;
	}
	return UnlockLeftDoor_RS;
}
void *OpenLeftDoor_RS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[LEFT_OPEN]) == 0) {
		strncpy(msg, outMessage[LEFT_DOOR_OPEN], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return CloseLeftDoor_RS;
	}
	return OpenLeftDoor_RS;
}

void* CloseLeftDoor_RS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[LEFT_CLOSED]) == 0) {
		strncpy(msg, outMessage[LEFT_DOOR_CLOSED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return LockLeftDoor_RS;
	}
	return CloseLeftDoor_RS;
}
void* LockLeftDoor_RS() {
	char msg[256];
	rcvidForInputs = MsgReceive(chidInput, &msgReceivedFromInputs,
			sizeof(msgReceivedFromInputs), NULL);
	if (rcvidForInputs == -1) {
		printf("Problem receiving message.\n");
		return EXIT_FAILURE;
	}
	int msgReplyStatusInputs = MsgReply(rcvidForInputs, 1, &replyToInputs,
			sizeof(replyToInputs));
	if (strcmp(msgReceivedFromInputs, inMessage[GAURD_LEFT_LOCK]) == 0) {
		strncpy(msg, outMessage[LEFT_DOOR_LOCKED], 256);
		int msgSendStatus = MsgSend(connectionIdToDisplay, msg,
				sizeof(msg), &replyFromDisplay, sizeof(replyFromDisplay));
		if (msgSendStatus == -1) {
			printf("Problem while sending the message to ass1_display");
			return EXIT_FAILURE;
		}
		return ScanId;
	}
	return LockLeftDoor_RS;
}

