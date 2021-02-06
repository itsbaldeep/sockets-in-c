#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int sockfd;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);

    // Creating the server socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("Socket initialization failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket initialized.\n");

    // Creating the server address
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    // Binding the socket to the address
    if (bind(sockfd, (struct sockaddr*) &addr, addrlen) < 0) {
        perror("Binding failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Binding successful.\n");

    // Listening to 3 connections at most
    if (listen(sockfd, 3) < 0) {
        perror("Listening failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Listening to client requests.\n");

    while (1) {
        // Accepting the client requests
        int newsock = accept(sockfd, (struct sockaddr*) &addr, (socklen_t*) &addrlen);
        if (newsock < 0) {
            perror("Accepting failure.\n");
            exit(EXIT_FAILURE);
        }
        printf("Client request accepted.\n");

        // Reading client request
        char buff[1024];
        int reslen = read(newsock, buff, 1024);
        printf("Client sent: ");
        for (int i = 0; i < reslen; i++) printf("%c", buff[i]);
        printf("\n");
        
        // Sending response to the client
        char *msg = "Hello.";
        send(newsock, msg, strlen(msg), 0);
        printf("Response sent: %s\n", msg);
    }
    
    // Closing the socket
    if (shutdown(sockfd, SHUT_RDWR) < 0) {
        perror("Shutdown failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket closed.\n");

} 
