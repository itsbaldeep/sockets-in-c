#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[]) {
    int sockfd;
    struct sockaddr_in servaddr;

    // Creating the client socket
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket initialization failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket initialized.\n");

    // Creating the server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);

    // Converting presentation format address to binary
    if(inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) < 0) {
        printf("Invalid address.\n"); 
        exit(EXIT_FAILURE);
    }
    printf("Address conversion successful.\n");

    // Connecting to the server
    if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failure.\n"); 
        exit(EXIT_FAILURE);
    }
    printf("Connected to the server.\n");

    // Sending request to server
    char *msg = "Say hello.";
    send(sockfd, msg, strlen(msg), 0);
    printf("Request sent: %s\n", msg);

    // Reading response from server
    char buff[1024];
    int reslen = read(sockfd, buff, 1024);
    printf("Response received: ");
    for (int i = 0; i < reslen; i++) printf("%c", buff[i]);
    printf("\n");

    // Closing the socket
    if (shutdown(sockfd, SHUT_RDWR) < 0) {
        perror("Shutdown failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket closed.\n");

}
