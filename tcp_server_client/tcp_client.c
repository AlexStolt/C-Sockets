#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdbool.h>

#define PORT "8080"
#define MAXDATASIZE 32

//Translate Address
void *address(struct sockaddr *sa){
  //IPv4
  if(sa->sa_family == AF_INET){
    return &((struct sockaddr_in *) sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *) sa)->sin6_addr;
}

int main(int argc, char const *argv[]){
  struct addrinfo hints, *server_info, *current = NULL;
  int sockfd;
  char ip_addr[INET6_ADDRSTRLEN], buffer[MAXDATASIZE];

  if(argc != 2){
    printf("./client <SERVER IP>\n");
    return -1;
  }

  //Set Connection Parameters
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM; //TCP


  //Get Server's Info
  if(getaddrinfo(argv[1], PORT, &hints, &server_info) != 0){
    perror("[ERROR]: getaddrinfo()");
    exit(EXIT_FAILURE);
  }

  //Connect with Socket to Server
  for(current = server_info; current != NULL; current = current->ai_next){
    if((sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol)) == -1){
      continue;
    }
    if(connect(sockfd, current->ai_addr, current->ai_addrlen) == -1){
      continue;
    }

    break;
  }

  if(!current){
    return -1;
  }

  //Log Connection to Server
  inet_ntop(current->ai_family, address((struct sockaddr *) current->ai_addr), ip_addr, INET6_ADDRSTRLEN);
  printf("Connecting to %s:%d\n", ip_addr, ntohs(((struct sockaddr_in *) current->ai_addr)->sin_port));

  freeaddrinfo(server_info);

  if(recv(sockfd, buffer, MAXDATASIZE - 1, 0) == -1){
    return -1;
  }

  printf("Message Recieved: %s\n", buffer);

  close(sockfd);

  return 0;
}
