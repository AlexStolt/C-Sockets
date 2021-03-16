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

#define PORT "8080"
#define MAXDATALEN 32



void *address(struct sockaddr *sa){
  if(sa->sa_family == AF_INET6){
    return &((struct sockaddr_in6 *) sa)->sin6_addr;
  }
  return &((struct sockaddr_in *) sa)->sin_addr;
}


int main(int argc, char const *argv[]){
  struct addrinfo hints, *server_info, *current;
  char ip_addr[INET6_ADDRSTRLEN], buffer[MAXDATALEN];
  int sockfd, bytes;
  struct sockaddr_storage client;
  socklen_t addr_len;
  
  //Set Server Hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET6;  //Use IPv6
  hints.ai_socktype = SOCK_DGRAM; //Use UDP
  hints.ai_flags = AI_PASSIVE;

  if(getaddrinfo(NULL, PORT, &hints, &server_info) != 0){
    perror("[ERROR]: getaddrinfo()");
    exit(EXIT_FAILURE);
  }


  //Loop Create Socket and Bind on the First Available
  for(current = server_info; current != NULL; current = current->ai_next){
    if((sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol)) < 0){
      perror("[ERROR]: socket()");
      continue;
    }
    if(bind(sockfd, current->ai_addr, current->ai_addrlen) < 0){
      perror("[ERROR]: bind()");
      continue;
    }
    break;
  }

  if(!current){
    perror("Socket Failed to Bind");
    exit(EXIT_FAILURE);
  }

  //Free server_info List Since Bind was Successful
  freeaddrinfo(server_info);

  //Listen For Incoming Connections
  addr_len = sizeof(client);
  if((bytes = recvfrom(sockfd, buffer, MAXDATALEN - 1, 0, (struct sockaddr *) &client,  &addr_len) == -1)){
    return -1;
  }

  //Display Client Address
  inet_ntop(client.ss_family, address((struct sockaddr *) &client), ip_addr, INET6_ADDRSTRLEN);
  printf("Client: %s Sent %s\n", ip_addr, buffer);


  close(sockfd);

  return 0;
}
