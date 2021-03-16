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

int main(int argc, char const *argv[]){
  struct addrinfo hints, *server_info, *current = NULL;
  int sockfd, bytes;

  if(argc != 2){
    printf("./client <SERVER IP>\n");
    return -1;
  }

  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_DGRAM;
  
  if(getaddrinfo(argv[1], PORT, &hints, &server_info) != 0){
    return -1;
  }

  for(current = server_info; current != NULL; current = current->ai_next){
    if((sockfd = socket(current->ai_family, current->ai_socktype, current->ai_protocol)) == -1){
      continue;
    }
    break;
  }

  if(!current){
    return -1;
  }

  if((bytes = sendto(sockfd, "Hello, World", 12, 0, current->ai_addr, current->ai_addrlen)) == -1){
    return -1;
  }


  freeaddrinfo(server_info);
  close(sockfd);

  return 0;
}
