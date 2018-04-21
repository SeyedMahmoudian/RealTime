/* File name: client.c
* Purpose: Client side of the program that will send a message to server
* 			it requires hostname and port number of the server
*  CST8244 - Real-Time Programming
*  Version: 1
*  Author:  Flaming Patel , Seyedamin Seyedmahmoudian
*  Date: 17 February 2018
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
/*
 * Function Name: error
 * Purpose: Print error message
 * param: char *
 * version: 1.0
 * author: MOHAMMAD PATOARY
 * return: void
 */
void error(char *msg) {
    perror(msg);
    exit(0);
}
/*
 * Function Name: main
 * Purpose: accept the host name and port number to connect to server.
 * 			once the connection was successful it will prompt for message to be typed
 * 			once the message was typed, it will send the message to server
 * 			close the connection.
 * param: int , char *
 * version:1
 * author:MOHAMMAD PATOARY
 * return: int
 */

int main(int argc, char *argv[]) {
    int sockfd, portno, n; //int variable to hold information for connection
    struct sockaddr_in serv_addr; // Structures which will hold information regarding the current connection
    struct hostent *server; // pointer to structures returned by network data base library
    char buffer[256];// buffer to hold the message to be send to server

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    portno = atoi(argv[2]);//get the port number form the argument
    sockfd = socket(AF_INET, SOCK_STREAM, 0);     //create end point communication
    if (sockfd < 0)//in case there was an issue with creating end point print the error
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);//get the host name from the second argument and store it in server variable
    if (server == NULL) { //check for null in case server name was not provided
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));//write 0 to memory making sure there are no information stored in memory to cause issues
    serv_addr.sin_family = AF_INET;//store family/type of the connection in structures
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);//convert 16 bit value to network-byte order, store it in structure sin port variable
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)//make connection to socket base on data provided
        error("ERROR connecting");//throw error in case the connection to socket failed
    printf("Please enter the message: ");
    bzero(buffer,256);//over write the buffer with zero to make sure there are no junk in the memory
    fgets(buffer,255,stdin);//store the message in buffer array
    n = write(sockfd,buffer,strlen(buffer));//write the buffer message to socket and send it to server
    if (n < 0)
         error("ERROR writing to socket");
    bzero(buffer,256);//over write the buffer with zero once agian
    n = read(sockfd,buffer,255);//read from the socket, reading the server message
    if (n < 0)
         error("ERROR reading from socket");
    printf("%s\n",buffer);//print the server message to consol
    return 0;
}
