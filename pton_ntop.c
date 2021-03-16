//Implementation of pton() and ntop()
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>


int main(int argc, char const *argv[]){
    struct sockaddr_in sa;
    char address[INET_ADDRSTRLEN];

    //Presentation to Network (IPv4)
    if(!inet_pton(AF_INET, "127.0.0.1", &sa.sin_addr.s_addr))
        return -1;

    printf("pton(): %d\n", sa.sin_addr.s_addr);
    
    //Network to Presentation (IPv4)
    if(!inet_ntop(AF_INET, &sa.sin_addr.s_addr, address, INET_ADDRSTRLEN))
        return -1;

    printf("ntop(): %s\n", address);

    return 0;
}
