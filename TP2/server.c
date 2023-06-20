#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>


#define PORT_SOURCE 5000
#define PORT_CLIENTS 5005
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

// Function to handle source sockets
void *handle_source(void *args) {
    int source_socket = *((int *)args);

    // Listen for incoming connections
    if (listen(source_socket, 5) == -1) {
        perror("Failed to listen for connections");
        exit(EXIT_FAILURE);
    }

    // Socket starts and listens for connections
    printf("Server listening on port 5000...\n");

    // Create a thread pool to handle client requests
    pthread_t threads[5];
    int numThreads = 0;

    return NULL;
}

// Function to handle clients sockets
void *handle_clients(void *args) {
    int clients_socket = *((int *)args);

    // Listen for incoming connections
    if (listen(clients_socket, 5) == -1) {
        perror("Failed to listen for connections");
        exit(EXIT_FAILURE);
    }

    // Socket starts and listens for connections
    printf("Server listening on port 5005...\n");

    return NULL;
}


int main(int argc, char const *argv[]) {

    int valread;

    struct sockaddr_in address_source;
    int addrlen_source = sizeof(address_source);

    struct sockaddr_in address_clients;
    int addrlen_clients = sizeof(address_clients);

    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Create a socket for source
    int *socket_source = (int *) malloc(sizeof(int));
    *socket_source = socket(AF_INET, SOCK_DGRAM, 0);
    if (*socket_source == -1) {
        perror("Failed to create socket_source");
        exit(EXIT_FAILURE);
    }

    // Create socket for clients
    int *socket_clients = (int *) malloc(sizeof(int));
    *socket_clients = socket(AF_INET, SOCK_DGRAM, 0);
    if (*socket_clients == -1) {
        perror("Failed to create socket_client");
        exit(EXIT_FAILURE);
    }

    // Define the server address for source
    struct sockaddr_in server_address_source;
    server_address_source.sin_family = AF_INET;
    server_address_source.sin_port = htons(PORT_SOURCE);  // Example port number
    server_address_source.sin_addr.s_addr = INADDR_ANY;

    // Define the server address for clients
    struct sockaddr_in server_address_clients;
    server_address_clients.sin_family = AF_INET;
    server_address_clients.sin_port = htons(PORT_CLIENTS);  // Example port number
    server_address_clients.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket_source to the specified address and port
    if (bind(*socket_source, (struct sockaddr*)&server_address_source, sizeof(server_address_source)) == -1) {
        perror("Failed to bind socket_source");
        exit(EXIT_FAILURE);
    }

    // Bind the socket_clients to the specified address and port
    if (bind(*socket_clients, (struct sockaddr*)&server_address_clients, sizeof(server_address_clients)) == -1) {
        perror("Failed to bind socket_clients");
        exit(EXIT_FAILURE);
    }

    pthread_t main_threads[2];

    // Start the thread to receive source
    if (pthread_create(&main_threads[1], NULL, handle_source, (void *) socket_source) != 0) {
        perror("Error creating thread");
        break;
    }

    // Start the thread to receive source
    if (pthread_create(&main_threads[2], NULL, handle_clients,  socket_clients) != 0) {
        perror("Error creating thread");
        break;
    }

    // Create a thread pool to handle client requests
    pthread_t threads[5];
    int numThreads = 0;

    while (true) {

        // Accept a new client to the server
        int *clientSocket = (int *) malloc(sizeof(int));
        if ((*clientSocket = accept(socket_source, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
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
    shutdown(socket_source, SHUT_RDWR);

    return 0;
}
