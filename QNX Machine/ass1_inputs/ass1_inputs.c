#include <stdlib.h>
#include <stdio.h>
#include <proj.h>

int main(int argc, char *argv[]) {

	char person_id[128];
	char personWeight[128];
	int reply;
	char event[10];
	FILE * fp;



	int connectionIdToController = ConnectAttach(0, atoi(argv[1]), 1, 0, 0);

	if (connectionIdToController == -1) {
		printf("Cannot Connect to Server. Please try again\n");
		return EXIT_FAILURE;
	}

	while (1) {
		printf("Enter the event type (LS= left scan,\n"
				" RS= right scan, WS= weight scale,\n"
				" LO =left open, RO=right open,\n"
				" LC = left closed, RC = right closed,\n"
				" GRU = guard right unlock, GRL = guard right lock,\n"
				" GLL=guard left lock, GRU = guard left unlock)\n");
		if(scanf("%s", event) == -1){
			return 0;
		}

		if (strcmp(event, "WS") == 0) {
			scanf("%s", &personWeight);
			strcat(event, " ");
			strcat(event, personWeight);

		}
		if (strcmp(event, "LS") == 0 || strcmp(event, "RS") == 0) {
			scanf("%s", &person_id);
			strcat(event, " ");
			strcat(event, person_id);
		}

		int msgSendStatus = MsgSend(connectionIdToController, &event, sizeof(event),
				&reply, sizeof(reply));

		if (msgSendStatus == -1) {
			printf("Problem while sending the message");
			return EXIT_FAILURE;
		}
		if (reply == -1) {
			printf("Server Error: Problem with reply from server");
		}
	}

	ConnectDetach(connectionIdToController);

	return EXIT_SUCCESS;
}
