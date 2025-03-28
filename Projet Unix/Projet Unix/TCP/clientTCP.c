#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12201
#define MAX_BUFFER_SIZE 1224



// Function to display the menu and get user choice
int displayMenuAndGetChoice() {
    int choice;
    printf("\nMenu:\n");
    printf("1. Get Date and Time\n");
    printf("2. Get File List\n");
    printf("3. Get File Content\n");
    printf("4. Get Elapsed Time\n");
    printf("5. Quit\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Consume the newline character left in the input buffer

    return choice;
}

// Function to receive and display the result from the server
void receiveAndDisplayResult(int clientSocket) {
    char resultBuffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead = recv(clientSocket, resultBuffer, sizeof(resultBuffer), 0);
    
    if (bytesRead > 0) {
        resultBuffer[bytesRead] = '\0'; // Null-terminate the received data
        printf("Result from the server:\n%s\n", resultBuffer);
    } else {
        printf("Error receiving result from the server.\n");
    }
}

int authenticate(int clientSocket) {
    char username[MAX_BUFFER_SIZE];
    char password[MAX_BUFFER_SIZE];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    // Send authentication information to the server
    send(clientSocket, username, strlen(username), 0);
    
    usleep(100000); 
    send(clientSocket, password, strlen(password), 0);

    usleep(100000); 
    // Receive authentication result from the server
    char authResult[MAX_BUFFER_SIZE];
    recv(clientSocket, authResult, sizeof(authResult), 0);
	
    if (strcmp(authResult, "AUTH_SUCCESS") == 0) {
        printf("Authentication successful.\n");
        return 1;
    } else {
        printf("Authentication failed. Exiting...\n");
        return 0;
    }
}
// Function to handle service selection and request sending
void handleServiceSelection(int clientSocket) {

	  if (!authenticate(clientSocket)) {
        close(clientSocket);
        exit(EXIT_FAILURE);
    }

    int choice;
    do {
        choice = displayMenuAndGetChoice();

        switch (choice) {
            case 1:
                send(clientSocket, "GET_DATETIME", strlen("GET_DATETIME"), 0);
                receiveAndDisplayResult(clientSocket);
                break;
            case 2:
                send(clientSocket, "GET_FILE_LIST", strlen("GET_FILE_LIST"), 0);
                char directoryPath[MAX_BUFFER_SIZE];
                printf("Enter Directory Path");
                scanf("%s", directoryPath);
               	send(clientSocket, directoryPath, strlen(directoryPath), 0);
                receiveAndDisplayResult(clientSocket);
                break;
            case 3:
                send(clientSocket, "GET_FILE_CONTENT", strlen("GET_FILE_CONTENT"), 0);
                char filePath[MAX_BUFFER_SIZE];
                printf("Enter File Path");
                scanf("%s", filePath);
               	send(clientSocket, filePath, strlen(filePath), 0);
                receiveAndDisplayResult(clientSocket);
                break;
            case 4:
                send(clientSocket, "GET_ELAPSED_TIME", strlen("GET_ELAPSED_TIME"), 0);
                receiveAndDisplayResult(clientSocket);
                break;
            case 5:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 5);
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_hostname> <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const char *serverHostname = argv[1];
    const int serverPort = atoi(argv[2]);

    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    if (inet_pton(AF_INET, serverHostname, &serverAddr.sin_addr) <= 0) {
        perror("Invalid server address");
        exit(EXIT_FAILURE);
    }

    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error connecting to the server");
        exit(EXIT_FAILURE);
    }

  
    // Handle service selection and requests
    handleServiceSelection(clientSocket);

    close(clientSocket);
    return 0;
}

