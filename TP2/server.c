#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT_SOURCE 5000
#define PORT_CLIENT 5005
#define BUFFER_SIZE 1024

int clientCount = 0;
pthread_mutex_t mutex;

// Struct to store source data
typedef struct {
    int socket;
    struct sockaddr_in address;
} ClientInfo;



void *source_Thread(void *args) {

    int socket = *(int *)args;
    char buffer[BUFFER_SIZE];

    printf("Server listening on port %d...\n", PORT_SOURCE);

    while(true) {

        while(read(socket, buffer, BUFFER_SIZE) != 0) {

            printf("%s\n", buffer);

        }

    }

    pthread_exit(NULL);
}


void *client_Thread(void *args) {

    int socket = *(int *)args;
    char buffer[BUFFER_SIZE];

    printf("Server listening on port %d...\n", PORT_CLIENT);

    while(true) {

        while(read(socket, buffer, BUFFER_SIZE) != 0) {

            printf("%s\n", buffer);

        }

    }

    pthread_exit(NULL);
}


int main(int argc, char const *argv[]) {

    // Initialize o mutex
    pthread_mutex_init(&mutex, NULL);

    // Create a socket for source
    int *socket_source = (int *) malloc(sizeof(int));
    *socket_source = socket(AF_INET, SOCK_DGRAM, 0);
    if (*socket_source == -1) {
        perror("Failed to create socket_source");
        exit(EXIT_FAILURE);
    }

    // Create a socket for clients
    int *socket_clients = (int *) malloc(sizeof(int));
    *socket_clients = socket(AF_INET, SOCK_DGRAM, 0);
    if (*socket_source == -1) {
        perror("Failed to create socket_clients");
        exit(EXIT_FAILURE);
    }

    // Define the server address for source
    struct sockaddr_in server_address_source;
    server_address_source.sin_family = AF_INET;
    server_address_source.sin_port = htons(PORT_SOURCE);  // Example port number
    server_address_source.sin_addr.s_addr = INADDR_ANY;

    // Define the server address for source
    struct sockaddr_in server_address_clients;
    server_address_clients.sin_family = AF_INET;
    server_address_clients.sin_port = htons(PORT_CLIENT);  // Example port number
    server_address_clients.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket_source to the specified address and port
    if (bind(*socket_source, (struct sockaddr*)&server_address_source, sizeof(server_address_source)) < 0) {
        perror("Failed to bind socket_source");
        exit(EXIT_FAILURE);
    }

    // Bind the socket_clients to the specified address and port
    if (bind(*socket_clients, (struct sockaddr*)&server_address_clients, sizeof(server_address_clients)) < 0) {
        perror("Failed to bind socket_clients");
        exit(EXIT_FAILURE);
    }

    pthread_t source_thread;
    pthread_t client_thread;


    // Create thread for source
    if(pthread_create(&source_thread, NULL, source_Thread, (void *)socket_source) != 0) {
        perror("Error creating thread");
        return 0;
    }

    // Create thread for clients
    if(pthread_create(&client_thread, NULL, client_Thread, (void *)socket_clients) != 0) {
        perror("Error creating thread");
        return 0;
    }

    pthread_join(source_thread, NULL);
    pthread_join(client_thread, NULL);

    // Close sockets
    close(*socket_source);
    close(*socket_clients);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
