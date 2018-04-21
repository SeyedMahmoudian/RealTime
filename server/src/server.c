/* File name: server.c
* Purpose:Server side of the program that will run in background and
* 		  acknowledge the when a message is received, it require a port number
*  CST8244 - Real-Time Programming
*  Version: 1
*  Author:   Seyedamin Seyedmahmoudian
*  Date: 17 February 2018
*/

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
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
    exit(1);
}
/*
 * Function Name: main
 * Purpose: open a socket on specified port, wait until a connection to the port has been made, then will print a message once it received it,
 * 			once the message was successfully received and printed it will send back the ack to the client stating the message was received.
 * param: int , char *
 * version:1
 * author:Seyedamin Seyedmahmoudian
 * return: int
 */

int main(int argc, char *argv[]) {
     int sockfd, newsockfd, portno, clilen; //int variable to hold information for connection
     char buffer[256]; // buffer to hold the message received from the client
     structures sockaddr_in serv_addr, cli_addr; // structures which will hold information regarding the current connection
     int n;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
     //create end point communication
     sockfd = socket(AF_INET, SOCK_STREAM, 0);
     if (sockfd < 0)//in case there was an issue with creating end point print the error
        error("ERROR opening socket");
     bzero((char *) &serv_addr, sizeof(serv_addr));//write 0 to memory making sure there are no information stored in memory to cause issues
     portno = atoi(argv[1]);//get the port number form the argument
     serv_addr.sin_family = AF_INET;//store family/type of the connection in structures
     serv_addr.sin_addr.s_addr = INADDR_ANY; // write zero to structures
     serv_addr.sin_port = htons(portno);//store port number in structures
     if (bind(sockfd, (structures sockaddr *) &serv_addr,
              sizeof(serv_addr)) < 0) //bind the addresses together to make the connection
              error("ERROR on binding");
     listen(sockfd,5);//start listening on the socket
     clilen = sizeof(cli_addr);//store the size of the client addres in the variable
     newsockfd = accept(sockfd, (structures sockaddr *) &cli_addr, &clilen);//accept the connection if there are no pending connection otherwise wait for pending connection
     if (newsockfd < 0)
          error("ERROR on accept");
     bzero(buffer,256);//write zero to buffer array to clean the memroy
     n = read(newsockfd,buffer,255);//read number of the byte in socket
     if (n < 0) error("ERROR reading from socket");
     printf("Here is the message: %s\n",buffer);//print the message
     n = write(newsockfd,"I got your message",18);//return the ack to client
     if (n < 0) error("ERROR writing to socket");
     return 0;
}
