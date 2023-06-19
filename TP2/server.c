#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>


#define PORT 5000
#define BUFFER_SIZE 1024


void *clientThread(void *arg) {
    char buffer[BUFFER_SIZE];
    int socket = *((int *) arg);
    int i = 0;

    while(i != 5) {

        //Receive data from the client
        read(socket, buffer, BUFFER_SIZE);
        printf("%s\n", buffer);

        i++;
    }

    close(socket);
    free(arg);
    return NULL;
}

int main(int argc, char const *argv[]) {

    int valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Create a socket
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5000);  // Example port number
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Failed to listen for connections");
        exit(EXIT_FAILURE);
    }


    // Socket starts and listens for connections
    printf("Server listening on port 5000...\n");

    // Create a thread pool to handle client requests
    pthread_t threads[5];
    int numThreads = 0;

    while (true) {

        // Accept a new client to the server
        int *clientSocket = (int *) malloc(sizeof(int));
        if ((*clientSocket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Start the thread to receive messages
        if (pthread_create(&threads[numThreads++], NULL, clientThread, (void *) clientSocket) != 0) {
            perror("Error creating thread");
            break;
        }

        // If total number of thread is reached close program
        if (numThreads >= 10) {
            printf("Maximum number of threads reached!");
            break;
        }

    }

    /*// Sends messages to client
    send(clientSocket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    */

    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);

    return 0;
}
