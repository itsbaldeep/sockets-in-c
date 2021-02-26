#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080

int main(int argc, char const *argv[]) {

    // Creating a client socket
    int sockfd;
    struct sockaddr_in servaddr;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket initialization failure.\n");
        exit(EXIT_FAILURE);
    }
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr) < 0) {
        printf("Invalid address.\n"); 
        exit(EXIT_FAILURE);
    }

    // Attempting to connect the socket to the server
    if (connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0) {
        perror("Connection failure.\n"); 
        exit(EXIT_FAILURE);
    }

    // Sending a random number to the server
    srand(time(0));
    int num = rand() % 100;
    int tmp = htonl(num);
    send(sockfd, &tmp, sizeof(tmp), 0);
    printf("Number sent: %d\n", num);

    // Getting the reply and priting it
    char buff[1024];
    int reslen = read(sockfd, buff, 1024);
    printf("Is this number a prime?: ");
    for (int i = 0; i < reslen; i++) printf("%c", buff[i]);
    printf("\n");

    // Terminating the socket
    if (shutdown(sockfd, SHUT_RDWR) < 0) {
        perror("Shutdown failure.\n");
        exit(EXIT_FAILURE);
    }
    printf("Socket closed.\n");

}
