#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>

#define PORT 12201
#define MAX_BUFFER_SIZE 1224
// Function to send date and time
void sendDateTime(int clientSocket) {
    time_t currentTime;
    time(&currentTime);
    struct tm *localTime = localtime(&currentTime);

    char datetime[MAX_BUFFER_SIZE];
    strftime(datetime, MAX_BUFFER_SIZE, "%Y-%m-%d %H:%M:%S", localTime);

    send(clientSocket, datetime, strlen(datetime), 0);
}

// Function to send a list of files in a directory
void sendFileList(int clientSocket, const char *directoryPath) {
    DIR *dir = opendir(directoryPath);
    
    if (!dir) {
        perror("Error opening directory");
        send(clientSocket, "Error opening directory" ,strlen("Error opening directory"), 0);
        return;
    }

    char fileList[MAX_BUFFER_SIZE] = "";
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        strcat(fileList, entry->d_name);
        strcat(fileList, "\n");
    }

    closedir(dir);

    send(clientSocket, fileList, strlen(fileList), 0);
}

// Function to send the content of a file
void sendFileContent(int clientSocket, const char *filePath) {
   printf("file path %s",filePath);
    int file = open(filePath, O_RDONLY);
    if (file == -1) {
        perror("Error opening file");
         send(clientSocket, "Error opening file", strlen("Error opening file"), 0);
    }

    char buffer[MAX_BUFFER_SIZE];
    ssize_t bytesRead;

    while ((bytesRead = read(file, buffer, sizeof(buffer))) > 0) {
        send(clientSocket, buffer, bytesRead, 0);
    }

    close(file);
}

void sendElapsedTime(int clientSocket, time_t startTime) {
    // Get the current time
    time_t currentTime;
    time(&currentTime);

    // Calculate the elapsed time in seconds
    double elapsedTime = difftime(currentTime, startTime);

    // Format the elapsed time as a string with two decimal places
    char elapsedStr[MAX_BUFFER_SIZE];
    snprintf(elapsedStr, MAX_BUFFER_SIZE, "%.2f seconds", elapsedTime);

    // Send the elapsed time string to the client
    ssize_t sentBytes = send(clientSocket, elapsedStr, strlen(elapsedStr), 0);

    // Check for errors in the send function
    if (sentBytes == -1) {
        perror("Error sending elapsed time");
    } else {
        printf("Elapsed time sent successfully: %s\n", elapsedStr);
    }
}


void handleClient(int clientSocket) {
   
    time_t startTime;
    time(&startTime);
    char request[MAX_BUFFER_SIZE];
    ssize_t bytesRead;
    
    // Authentication
    char username[MAX_BUFFER_SIZE];
    char password[MAX_BUFFER_SIZE];

    // Receive username and password from the client
    bytesRead = recv(clientSocket, username, sizeof(username), 0);
    if (bytesRead <= 0) {
        perror("Error receiving username");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    username[bytesRead] = '\0';
	
    usleep(100000); 
    bytesRead = recv(clientSocket, password, sizeof(password), 0);
    if (bytesRead <= 0) {
        perror("Error receiving password");
        close(clientSocket);
        exit(EXIT_FAILURE);
    }
    password[bytesRead] = '\0';

    // Perform authentication
    if ((strcmp(password, "password") == 0) && (strcmp(username, "user") == 0)) {
        send(clientSocket, "AUTH_SUCCESS", strlen("AUTH_SUCCESS"), 0);
    } else {
        send(clientSocket, "AUTH_FAILED", strlen("AUTH_FAILED"), 0);
        close(clientSocket);
        exit(EXIT_SUCCESS);
    }

    while ((bytesRead = recv(clientSocket, request, sizeof(request), 0)) > 0) {
        request[bytesRead] = '\0'; 
	printf("%s\n",request);
        // Handle different types of requests
        if (strcmp(request, "GET_DATETIME") == 0) {
            sendDateTime(clientSocket);
        } else if (strcmp(request, "GET_FILE_LIST") == 0) {
        
            char directoryPath[MAX_BUFFER_SIZE];
            bytesRead = recv(clientSocket, directoryPath, sizeof(directoryPath), 0);
            if (bytesRead > 0) {
                directoryPath[bytesRead] = '\0'; 
                sendFileList(clientSocket, directoryPath); 
              
            }
            
        }else if (strcmp(request, "GET_FILE_CONTENT") == 0) {
           
            char filePath[MAX_BUFFER_SIZE];
            printf("%s",filePath);
            bytesRead = recv(clientSocket, filePath, sizeof(filePath), 0);
            if (bytesRead > 0) {
                filePath[bytesRead] = '\0'; 
                sendFileContent(clientSocket, filePath);
            }
            
        } else if (strcmp(request, "GET_ELAPSED_TIME") == 0) {

            sendElapsedTime(clientSocket, startTime);
        } else {
            printf("Unknown request: %s\n", request);
        }
    }

    close(clientSocket);
    exit(EXIT_SUCCESS);
    
}



int main(int argc, char *argv[]) {

if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    const int serverPort = atoi(argv[1]);

    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("Error creating socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(serverPort);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Error binding socket");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) == -1) {
        perror("Error listening");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", serverPort);

    while (1) {
        struct sockaddr_in clientAddr;
        socklen_t clientAddrLen = sizeof(clientAddr);
        int clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &clientAddrLen);
        if (clientSocket == -1) {
            perror("Error accepting connection");
            continue;
        }

        pid_t childPid = fork();
        if (childPid == -1) {
            perror("Error forking");
            close(clientSocket);
            continue;
        } else if (childPid == 0) {
            // Child process
            printf("New user connected to the server. Child PID: %d\n", getpid());
            close(serverSocket); // Child doesn't need the listening socket
            handleClient(clientSocket);
        } else {
         
            close(clientSocket); // Parent doesn't need the connected socket
         
        }
    }

    close(serverSocket);
    return 0;
}

