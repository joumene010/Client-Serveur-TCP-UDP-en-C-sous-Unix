#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int compare_integers(const void *a, const void *b) {
    return (*(int *)a - *(int *)b);
}


int main(int argc, char *argv[]) {
 	
   if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const int server_port = atoi(argv[1]);
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

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error binding socket");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", server_port);

    while (1) {
        // Receive n from the client
      
        int n;
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        ssize_t bytesReceived = recvfrom(sockfd, &n, sizeof(n), 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytesReceived < 0) {
            perror("Error receiving data from client");
            continue;
        }

        printf("Received %d from client\n ", n);
       
        int result[n];
        
        for (int i = 0; i < n; ++i) {
         result[i] = rand() % 100+ 1;
         }

	// Sort the numbers
	qsort(result, n, sizeof(int), compare_integers);
	for(int i =0 ;i<n ;i++){
		printf("%d\n",result[i]);
	}
        // Send the result to the client
        sendto(sockfd, &result, sizeof(result), 0, (struct sockaddr *)&client_addr, addr_len);
        printf("Sent result to client");
    }

    // Close the socket (not reached in this example)
    close(sockfd);

    return 0;
}

