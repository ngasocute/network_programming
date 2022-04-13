// #include <stdlib.h>
// #include <stdio.h>
// #include <sys/types.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <string.h>
// #include <unistd.h>
// #include <fcntl.h>
// #include <arpa/inet.h>

// #define MAXLINE 4096 /*max text line length*/
// #define SERV_PORT 3000 /*port*/
// #define LISTENQ 8 /*maximum number of client connections */

// int main (int argc, char **argv)
// {
//  int listenfd, connfd, n;
//  pid_t childpid;
//  socklen_t clilen;
//  char buf[MAXLINE];
//  struct sockaddr_in cliaddr, servaddr;
	
//  //creation of the socket
//  listenfd = socket (AF_INET, SOCK_STREAM, 0);
	
//  //preparation of the socket address 
//  servaddr.sin_family = AF_INET;
//  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
//  servaddr.sin_port = htons(SERV_PORT);
	
//  bind (listenfd, (struct sockaddr *) &servaddr, sizeof(servaddr));
	
//  listen (listenfd, LISTENQ);
	
//  printf("%s\n","Server running...waiting for connections.");
	
//  for ( ; ; ) {
		
//   clilen = sizeof(cliaddr);
//   connfd = accept (listenfd, (struct sockaddr *) &cliaddr, &clilen);
//   printf("%s\n","Received request...");
				
//   while ( (n = recv(connfd, buf, MAXLINE,0)) > 0)  {
//    printf("%s","String received from and resent to the client:");
//    puts(buf);
//    send(connfd, buf, n, 0);
//   }
			
//  if (n < 0) {
//   perror("Read error"); 
//   exit(1);
//  }
//  close(connfd);
	
//  }
//  //close listening socket
//  close (listenfd); 
// }
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#define SIZE 1024

void write_file(int sockfd){
  int n;
  FILE *fp;
  char *filename = "recv.txt";
  char buffer[SIZE];

  fp = fopen(filename, "w");
  while (1) {
    n = recv(sockfd, buffer, SIZE, 0);
    if (n <= 0){
      break;
      return;
    }
    fprintf(fp, "%s", buffer);
    bzero(buffer, SIZE);
  }
  return;
}

int main(){
  char *ip = "127.0.0.1";
  int port = 8080;
  int e;

  int sockfd, new_sock;
  struct sockaddr_in server_addr, new_addr;
  socklen_t addr_size;
  char buffer[SIZE];

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0) {
    perror("[-]Error in socket");
    exit(1);
  }
  printf("[+]Server socket created successfully.\n");

  server_addr.sin_family = AF_INET;
  server_addr.sin_port = port;
  server_addr.sin_addr.s_addr = inet_addr(ip);

  e = bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  if(e < 0) {
    perror("[-]Error in bind");
    exit(1);
  }
  printf("[+]Binding successfull.\n");

  if(listen(sockfd, 10) == 0){
		printf("[+]Listening....\n");
	}else{
		perror("[-]Error in listening");
    exit(1);
	}

  addr_size = sizeof(new_addr);
  new_sock = accept(sockfd, (struct sockaddr*)&new_addr, &addr_size);
  write_file(new_sock);
  printf("[+]Data written in the file successfully.\n");

  return 0;
}