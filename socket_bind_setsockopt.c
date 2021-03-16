#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>

int main(int argc, char const *argv[]){
  struct addrinfo hints, *res;
  int socket_fd;
  int allow = 1;

  //Hints For Server
  memset(&hints, 0, sizeof(hints));
  hints.ai_socktype = SOCK_STREAM; //TCP
  hints.ai_family = AF_UNSPEC; //IPv4 or IPv6
  hints.ai_flags = AI_PASSIVE;  //IP (Local Server)

  if(getaddrinfo(NULL, "8080", &hints, &res) != 0){
    return -1;
  }

  //Create a Socket
  if((socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0){
    return -1;
  }

  //Bind the Socket to a Certain Port
  if(bind(socket_fd, res->ai_addr, res->ai_addrlen) < 0){
    return -1;
  }

  //Allow Port to be Reused
  if(setsockopt(socket_fd, SOL_SOCKET, SO_REUSEADDR, &allow, sizeof(int)) < 0){
    return -1;
  }

  printf("Hello");
  return 0;
}
