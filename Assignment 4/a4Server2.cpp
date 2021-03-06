// Name: Hritik Panchasara
// netID: hhp160130
// Description: Concurrent server with fork, using sample base code from Part1 Assignment 4. 

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 80 /*port*/
#define LISTENQ 8 /*maximum number of client connections*/

using namespace std;

int main (int argc, char **argv){
  int listenfd, connfd, n;
  pid_t childpid;
  socklen_t clilen;
  char buf[MAXLINE];
  struct sockaddr_in cliaddr, servaddr;
   
  //Create a socket for the socket
  //If sockfd<0 there was an error in the creation of the socket
  if ((listenfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
  	perror("Problem in creating the socket");
  	exit(2);
  }

  //preparation of the socket address
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(atoi(argv[1]));  // Converting to big-endian order

  //bind the socket
  bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));

  //listen to the socket by creating a connection queue, then wait for clients
  listen (listenfd, LISTENQ);

  printf("%s\n","Server running...waiting for connections.");
  
  ofstream outputfile;
  outputfile.open("Output_Results.txt");
  
  for ( ; ; ) {
  clilen = sizeof(cliaddr);
  //accept a connection

  connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
  printf("%s\n","Received request...");

  if ( (childpid = fork ()) == 0 ) {	//if it’s 0, it’s child process
 	  printf ("%s\n","Child created for dealing with client requests");
   	//close listening socket
	  close (listenfd);

    while ( (n = recv(connfd, buf, MAXLINE,0)) > 0) {
      printf("My process ID : %d\n", getpid());
     	printf("%s","String received from and resent to the client:");
     	puts(buf);
      char arar[MAXLINE];
      strcpy(arar, buf);
      strcat(arar, " >> Output_Results.txt");
      system(arar);
      system(buf);
      memset(buf, 0, 255);
     	send(connfd, buf, n, 0);
    }
    
    if (n < 0)
      	printf("%s\n", "Read error");
     
     exit(0);
  }
  outputfile.close();
  
  //close socket of the server
  close(connfd);
 }
}