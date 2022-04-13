// #include <stdlib.h>
// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <arpa/inet.h>

// #define MAXLINE 4096 /*max text line length*/
// #define SERV_PORT 3000 /*port*/

// int
// main(int argc, char **argv) 
// {
//  int sockfd;
//  struct sockaddr_in servaddr;
//  char sendline[MAXLINE], recvline[MAXLINE];
	
//  //basic check of the arguments
//  //additional checks can be inserted
//  if (argc !=2) {
//   perror("Usage: TCPClient <IP address of the server"); 
//   exit(1);
//  }
	
//  //Create a socket for the client
//  //If sockfd<0 there was an error in the creation of the socket
//  if ((sockfd = socket (AF_INET, SOCK_STREAM, 0)) <0) {
//   perror("Problem in creating the socket");
//   exit(2);
//  }
	
//  //Creation of the socket
//  memset(&servaddr, 0, sizeof(servaddr));
//  servaddr.sin_family = AF_INET;
//  servaddr.sin_addr.s_addr= inet_addr(argv[1]);
//  servaddr.sin_port =  htons(SERV_PORT); //convert to big-endian order
	
//  //Connection of the client to the socket 
//  if (connect(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr))<0) {
//   perror("Problem in connecting to the server");
//   exit(3);
//  }
	
//  while (fgets(sendline, MAXLINE, stdin) != NULL) {
	
//   send(sockfd, sendline, strlen(sendline), 0);
		
//   if (recv(sockfd, recvline, MAXLINE,0) == 0){
//    //error: server terminated prematurely
//    perror("The server terminated prematurely"); 
//    exit(4);
//   }
//   printf("%s", "String received from the server: ");
//   fputs(recvline, stdout);
//  }

//  exit(0);
// }

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void send_file(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};

  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd;
  struct sockaddr_in server_addr;
  FILE *fp;
  char *filename = "send.txt";

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = connect(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e == -1) {
    perror("[-]Error in socket");
    exit(1);
  }
	printf("[+]Connected to Server.\n");

  fp = fopen(filename, "r");
  if (fp == NULL) {
    perror("[-]Error in reading file.");
    exit(1);
  }

  send_file(fp, sockfd);
  printf("[+]File data sent successfully.\n");

	printf("[+]Closing the connection.\n");
  close(sockfd);

  return 0;
}