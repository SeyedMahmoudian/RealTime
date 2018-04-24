#include <stdlib.h>
#include <stdio.h>
//#ifndef PROJ_H_
#include <proj.h>
//#endif
int main(int argc, char *argv[]) {

	int RcvIdForOutput, InputChannelId;
	char msg[256];

	int reply = 0; // 0 for success, -1 for failure
	printf("PID : %d : Running...\n", getpid());

	InputChannelId = ChannelCreate(0); //creates a channel that is used to receive messages and pulses

	while (1) {
		RcvIdForOutput = MsgReceive(InputChannelId, &msg, sizeof(msg), NULL);

		if (RcvIdForOutput == -1) {
			printf("Problem receiving message\n");
			return EXIT_FAILURE;
		}
		printf("%s\n", msg);

		int msgReplyStatus = MsgReply(RcvIdForOutput, 1, &reply, sizeof(reply));
		if (msgReplyStatus == -1) {
			printf("Problem replying to message\n");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}
