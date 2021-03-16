//Implementation of getaddrinfo()
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(int argc, char *argv[]){
	struct addrinfo hints, *result = NULL;
	int status;
	char *version;
	struct sockaddr_in *ipv4_addr;
	struct sockaddr_in6 *ipv6_addr;
	void *address;
	char ip[INET6_ADDRSTRLEN];
	
	
	if(argc != 2){
		perror("[ERROR]: showip <hostname>");
		return -1;
	}

	//Initialize Hints
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	//IPv4 or IPv6
	hints.ai_protocol = SOCK_STREAM; //TCP

	//Initialize Result to Point to List Head
	if((status = getaddrinfo(argv[1], NULL, &hints, &result)) != 0){
		perror("[ERROR]: getaddrinfo()");
		return -1;
	}

	for(struct addrinfo *current = result; current != NULL; current = current->ai_next){
		
		//Handle IPv4 Address
		if(current->ai_family == AF_INET){
			version = "IPv4";
			ipv4_addr = (struct sockaddr_in *) current->ai_addr;
			address = &(ipv4_addr->sin_addr);
		}
		else if(current->ai_family == AF_INET6) {
			version = "IPv6";
			ipv6_addr = (struct sockaddr_in6 *) current->ai_addr;
			address = &(ipv6_addr->sin6_addr);
		}

		inet_ntop(current->ai_family, address, ip, INET6_ADDRSTRLEN * sizeof(char));
		printf("%s: %s\n", version, ip);
	}

	return 0;
}