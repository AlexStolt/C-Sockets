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
#define ENABLED 0x01
#define BACKLOG 10

//Translate Address
void *address(struct sockaddr *sa){
  //IPv4
  if(sa->sa_family == AF_INET){
    return &((struct sockaddr_in *) sa)->sin_addr;
  }
  return &((struct sockaddr_in6 *) sa)->sin6_addr;
}


int main(int argc, char const *argv[]){
  struct addrinfo hints, *server_info, *current;
  int sock_fd, enbable = ENABLED, client_fd;
  struct sockaddr_in *info;
  struct sockaddr_storage client;
  socklen_t sin_size;
  char ip_addr[INET6_ADDRSTRLEN];

  //Set Sever Hints
  memset(&hints, 0, sizeof(struct addrinfo));
  hints.ai_family = AF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;


  if(getaddrinfo(NULL, PORT, &hints, &server_info) != 0){
    perror("[ERROR]: getaddrinfo()");
    exit(EXIT_FAILURE);
  }
  
  //Bind the First Available Socket
  for(current = server_info; current != NULL; current = current->ai_next){
    //Create Socket
    if((sock_fd = socket(current->ai_family, server_info->ai_socktype, server_info->ai_protocol)) == -1){
      perror("[ERROR]: socket()");
      continue;
    }

    //Manipulate Socket at API Level
    if(setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enbable, sizeof(int)) == -1){
      perror("[ERROR]: setsockopt()");
      continue;
    }

    //Bind Socket to Port
    if(bind(sock_fd, current->ai_addr, current->ai_addrlen) == -1){
      perror("[ERROR]: bind()");
      continue;
    }

    break;
  }

  //Server Failed to Bind
  if(!current){
    perror("[ERROR]: Server Failed to Bind");
    exit(EXIT_FAILURE);
  }

    //Free Server Info List
  freeaddrinfo(server_info);

  //Listen to Port
  if(listen(sock_fd, BACKLOG) == -1){
    perror("[ERROR]: listen()");
    exit(EXIT_FAILURE);
  }


  //Get Port
  info = (struct sockaddr_in *) current->ai_addr;
  printf("Server Listening on Port: %d\n", ntohs(info->sin_port));


  //Accept Loop
  while(true){
    sin_size = sizeof(client);    
    if((client_fd = accept(sock_fd, (struct sockaddr *) &client, &sin_size)) == -1){
      fprintf(stderr, "[ERROR]: accept()");
      continue;
    }


    //Display IP Address of Client
    inet_ntop(client.ss_family, address((struct sockaddr *) &client), ip_addr, INET6_ADDRSTRLEN);
    printf("Client Socket: %s:%d\n", ip_addr, ntohs(((struct sockaddr_in *) &client)->sin_port));
    fflush(stdout);

      //Create a Child Process
      if (!fork()){
        //Child Does NOT Need the Listener
        close(sock_fd); 
        if (send(client_fd, "Hello, World", 12, 0) == -1)
          perror("[ERROR]: send()");

        close(client_fd);
        return 0;
      }

      //Parent Does NOT Need Child Socket
      close(client_fd);  
    }
  
  return 0;
}
