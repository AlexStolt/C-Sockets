#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[]){
  struct addrinfo hints, *res;
  int socket_fd;
  char ip[INET6_ADDRSTRLEN];


  //Set Hints
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  

  if(getaddrinfo("www.google.com", "http", &hints, &res) != 0){
    perror("[ERROR]");
    return -1;
  }

  socket_fd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

  //Connect to Google
  connect(socket_fd, res->ai_addr, res->ai_addrlen);

  
  //Print IP and Version
  for(struct addrinfo *current = res; current != NULL; current = current->ai_next){
    if(current->ai_family == AF_INET){
      
      inet_ntop(AF_INET, (struct sockaddr_in *) current->ai_addr, ip, INET6_ADDRSTRLEN);
      printf("IPv4: %s\n", ip);
    }
    else if(current->ai_family == AF_INET6){
      inet_ntop(AF_INET6, (struct sockaddr_in *) current->ai_addr, ip, INET6_ADDRSTRLEN);
      printf("IPv6: %s\n", ip);
    }
  }


  return 0;
}
