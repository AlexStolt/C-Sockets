#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PORT "8080"
#define BACKLOG 10

int main(int argc, char const *argv[]){
  struct addrinfo hints, *res;
  struct sockaddr_storage client;
  socklen_t address_size;


  //Set Hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;

  //Address for Server
  if(getaddrinfo(NULL, PORT, &hints, &res) != 0){
    perror("Error at getaddrinfo()");
    return -1;
  }


  //Create Socket
  int socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);


  //Bind Socket to Port
  bind(socket_fd, res->ai_addr, res->ai_addrlen);


  //Listen for Connections
  listen(socket_fd, BACKLOG);

  printf("Listening on Port: %s\n", PORT);
  fflush(stdout);

  //Accept Requests 
  address_size = sizeof(client);
  int client_fd = accept(socket_fd, (struct sockaddr *) &client, &address_size);

  return 0;
}
