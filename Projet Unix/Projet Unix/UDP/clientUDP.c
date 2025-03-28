#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>



#define NMAX 20
int main(int argc, char *argv[]) {

   if (argc != 3) {
      printf(stderr, "Usage: %s <server_hostname> <server_port>\n", argv[0]);
      exit(EXIT_FAILURE);
    }
    
    const char *server_hostname = argv[1];
    const int server_port = atoi(argv[2]);
    
    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    // Set up server address
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(server_port);
    inet_pton(AF_INET, server_hostname, &server_addr.sin_addr);
    
    // generate random n value 
    int n;
    n= rand() % 20+ 1;
    printf("n: %d\n", n);
    
    // Send n to the server
    sendto(sockfd, &n, sizeof(n), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
    printf("message sent to the server %d\n", n);

    // Receive the result from the server
    int result[n];
    ssize_t bytesReceived = recvfrom(sockfd, &result, sizeof(result), 0, NULL, NULL);
    if (bytesReceived < 0) {
        perror("Error receiving data from server\n");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // Print the result
    printf("Sorted numberes received from server\n");
    for(int i=0 ; i<n ; i++)
     printf("Number %d : %d\n", i , result[i]);

    // Close the socket
    close(sockfd);

    return 0;
}

