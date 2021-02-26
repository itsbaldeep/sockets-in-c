#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    // Creating the server socket
    int sockfd;
    struct sockaddr_in addr;
    int addrlen = sizeof(addr);
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) { 
        perror("Socket initialization failure.\n");
        exit(EXIT_FAILURE);
    }
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);
    if (bind(sockfd, (struct sockaddr*) &addr, addrlen) < 0) {
        perror("Binding failure.\n");
        exit(EXIT_FAILURE);
    }

    // Listening to client requests
    if (listen(sockfd, 3) < 0) {
        perror("Listening failure.\n");
        exit(EXIT_FAILURE);
    }
    while (1) {
        int newsock = accept(sockfd, (struct sockaddr*) &addr, (socklen_t*) &addrlen);
        if (newsock < 0) {
            perror("Accepting failure.\n");
            exit(EXIT_FAILURE);
        }

        // Reading the number
        int num;
        int reslen = read(newsock, &num, sizeof(num));
        num = ntohl(num);
        printf("Client sent: %d\n", num);

        // Checking if it is prime or not
        int prime = 1;
        for (int i = 2; i * i <= num; i++) {
            if (num % i == 0) prime = 0;
        }
        
        // Sending the result to client
        char *msg = prime ? "Yes." : "No.";
        send(newsock, msg, strlen(msg), 0);
        printf("Response sent: %s\n", msg);
    }
    
    // Terminating the socket
    if (shutdown(sockfd, SHUT_RDWR) < 0) {
        perror("Shutdown failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket closed.\n");

} 
