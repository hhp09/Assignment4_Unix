// Name: Hritik Panchasara
// netID: hhp160130
// Description: Client .c file for Part 1, Assignment 4, using sample base code provided. 

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <ctime>
#define MAXLINE 4096 /*max text line length*/
#define SERV_PORT 10010 /*port*/

using namespace std;

/*
  OPEN A FILE
  READ LINE BY LINE
  SEND COMMAND TO SERVER
  CREATE NEW FILE. RECEIVE OUTPUT FROM SERVER
  PLACE OUTPUT INTO OTHER FILE
*/

int main(int argc, char **argv) 
{
     int sockfd;
     string strff;
     struct sockaddr_in servaddr;
     char sendline[MAXLINE], recvline[MAXLINE];
     char* strarg = argv[3];        // strarg stores third parameter when user runs the .cpp file
      
      alarm(300);  // to terminate after 5 minutes ---> "Alarm Clock"
    	
     //basic check of the arguments
     //additional checks can be inserted
    /* if (argc !=3) {
          perror("Usage: TCPClient <Server IP> <Server Port>"); 
          exit(1);
     }*/
    	
     //Create a socket for the client
     //If sockfd<0 there was an error in the creation of the socket
     if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
          perror("Problem in creating the socket");
          exit(2);
     }
    	
     //Creation of the socket
     memset(&servaddr, 0, sizeof(servaddr));
     servaddr.sin_family = AF_INET;
     servaddr.sin_addr.s_addr= inet_addr(argv[1]);
     servaddr.sin_port =  htons(atoi(argv[2]));     // Converting to big-endian order
     
    	
     //Connection of the client to the socket 
     if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
          perror("Problem in connecting to the server");
          exit(3);
     }
     
     //Open File here
     ifstream inputfile;
     ofstream outputfile;
     
     inputfile.open(strarg);                   // this argument will consist of the name of a .txt file from which the commands will be read
     outputfile.open("Output_Results.txt");    // Outputting results to desired, pre-set text file

     while (getline (inputfile,strff)) {
        string str_plchldr="end";
        if (strff.compare(str_plchldr) == 0) {
          cout << "Client process ended." <<  endl;
          exit(EXIT_FAILURE);
        }
        
        time_t now = time(0);
       	char* dt = ctime(&now);
        send(sockfd, strff.c_str(), strlen(strff.c_str()), 0);
            		
        if (recv(sockfd, recvline, MAXLINE,0) == 0){
         //error: server terminated prematurely
         perror("The server terminated prematurely"); 
         exit(4);
        }
        
        printf("My process ID : %d\n", getpid());
        cout << "The local date and time is: " << dt << endl;
        printf("%s", "String received from the server: ");
        
        fputs(recvline, stdout);

        printf("%s", "Transaction Recieved! \n");
        memset(recvline, 0, 255);
        cin.clear();
        strff.clear();
     }
     
     inputfile.close();
     outputfile.close();
     exit(0);
}