#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#include <pthread.h>

#define PORT_SOURCE 5000
#define PORT_CLIENT 5005
#define BUFFER_SIZE 1024

typedef struct {
    char *clients_id[5];
    int F;
    int N;
    int M;
} source;

source src[5];

int num_sources = 0;
int num_clients = 0;
char *channel_id[5];

pthread_mutex_t mutex;

struct sockaddr_in clientAddress;
socklen_t clientAddressLength = sizeof(clientAddress);


void *source_Thread(void *args) {

    int socket_t = *(int *) args;
    char buffer[BUFFER_SIZE];
    char buffer_to_send[BUFFER_SIZE];
    char *string;
    bool condition = false;

    // Create a socket_t for source
    int socket_source = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_source == -1) {
        perror("Failed to create socket_source");
        exit(EXIT_FAILURE);
    }

    // Define the server address for source
    struct sockaddr_in server_address_source;
    server_address_source.sin_family = AF_INET;
    server_address_source.sin_port = htons(PORT_SOURCE);  // Example port number
    server_address_source.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket_source to the specified address and port
    if (bind(socket_source, (struct sockaddr *) &server_address_source, sizeof(server_address_source)) < 0) {
        perror("Failed to bind socket_source");
        exit(EXIT_FAILURE);
    }


    printf("Server listening on port %d...\n", PORT_SOURCE);

    while (true) {

        // Receives from the source
        while (read(socket_source, buffer, BUFFER_SIZE) != 0) {
            char *copy = strdup(buffer);
            char *token = strtok(copy, "|");
            int index = 0;

            // Lock mutex to store content
            pthread_mutex_lock(&mutex);

            // Search the channel id in the array
            for (int i = 0; i < num_sources; i++) {
                if (strcmp(channel_id[i], token) == 0) {
                    condition = true;
                    index = i;
                }
            }
            // If not found, adds the channel id to the array
            if (condition == false) {
                channel_id[num_sources] = token;

                for (size_t j = 0; j < 6; j++) {
                    token = strtok(NULL, "|");
                    if (j == 3) {
                        src[num_sources].F = atoi(token);
                    } else if (j == 4) {
                        src[num_sources].N = atoi(token);
                    } else if (j == 5) {
                        src[num_sources].M = atoi(token);
                    }
                }

                num_sources++;
                index = num_sources;
                printf("Num_sources: %d \n", num_sources);
            }

            // Check if there's any client subscribed to the channel to send the information
            for (size_t i = 0; i < num_clients; i++) {
                if (src[index].clients_id[i] != NULL) {
                    string = src[index].clients_id[i];
                    sprintf(buffer_to_send, "%s %s" , string, buffer);

                    // Sends the information with the ID_Cliente in the beginning
                    if (sendto(socket_t, buffer_to_send, sizeof(buffer_to_send), 0,
                               (struct sockaddr *) &clientAddress,
                               clientAddressLength) == -1) {
                        perror("ERROR!");
                        exit(0);
                    }

                }
            }

            condition = false;

            // Unlock mutex to store content
            pthread_mutex_unlock(&mutex);
        }

    }

    close(socket_source);
    close(socket_t);
    pthread_exit(NULL);
}


void *client_Thread(void *args) {

    int socket = *(int *) args;
    char buffer[BUFFER_SIZE];
    char buffer_to_send[BUFFER_SIZE];
    char *token;

    // Define the server address for source
    struct sockaddr_in server_address_clients;
    server_address_clients.sin_family = AF_INET;
    server_address_clients.sin_port = htons(PORT_CLIENT);  // Example port number
    server_address_clients.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket_clients to the specified address and port
    if (bind(socket, (struct sockaddr *) &server_address_clients, sizeof(server_address_clients)) < 0) {
        perror("Failed to bind socket_clients");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT_CLIENT);


    while (true) {

        while (recvfrom(socket, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &clientAddress, &clientAddressLength) !=
               0) {
            char *copy = strdup(buffer);
            token = strtok(copy, " ");

            printf("%s\n", token);

            if (strcmp(token, "list") == 0) {   // If data received is "list"

                // Lock mutex to store content
                pthread_mutex_lock(&mutex);

                // Sends response to the client
                sprintf(buffer_to_send, "response-list|%d", num_sources);
                if (sendto(socket, buffer_to_send, 16, 0, (struct sockaddr *) &clientAddress, clientAddressLength) ==
                    -1) {
                    perror("ERROR!");
                    exit(0);
                }

                // Sends all the channels available to the client
                for (size_t i = 0; i < (size_t) num_sources; i++) {
                    sprintf(buffer_to_send, "%zu. %s", (i + 1), channel_id[i]);
                    if (sendto(socket, buffer_to_send, 20, 0, (struct sockaddr *) &clientAddress,
                               clientAddressLength) == -1) {
                        perror("ERROR!");
                        exit(0);
                    }

                }

                // Unlock mutex to store content
                pthread_mutex_unlock(&mutex);

            } else if (strcmp(token, "info") == 0) {    // If data received is "info (D)"
                int index = 0;
                token = strtok(NULL, " ");

                // Lock mutex to store content
                pthread_mutex_lock(&mutex);

                // Search for the channel in the array
                for (int i = 0; i < num_sources; i++) {
                    if (strcmp(channel_id[i], token) == 0) {
                        index = i;
                    }
                }

                // Send the response to the client with the information about the channel
                sprintf(buffer_to_send, "response-info,(%s|%d|%d|%d)", channel_id[index], src[index].F, src[index].N,
                        src[index].M);
                if (sendto(socket, buffer_to_send, 35, 0, (struct sockaddr *) &clientAddress,
                           clientAddressLength) == -1) {
                    perror("ERROR!");
                    exit(0);
                }

                // Unlock mutex to store content
                pthread_mutex_unlock(&mutex);


            } else if (strcmp(token, "play") == 0) {    // If data received is "play (D)"
                int index = -1;
                char * source_information;
                token = strtok(NULL, " ");

                // Lock mutex to store content
                pthread_mutex_lock(&mutex);

                // Search for the channel in the array
                for (int i = 0; i < num_sources; i++) {
                    if (strcmp(channel_id[i], token) == 0) {
                        index = i;
                    }
                }

                // Unlock mutex to store content
                pthread_mutex_unlock(&mutex);

                if (index == -1) {
                    sprintf(buffer_to_send, "channel does not exists...");
                    if (sendto(socket, buffer_to_send, 30, 0, (struct sockaddr *) &clientAddress,
                               clientAddressLength) == -1) {
                        perror("ERROR!");
                        exit(0);
                    }
                    break;
                } else {
                    // Send the response tro the client
                    sprintf(buffer_to_send, "response-play");
                    if (sendto(socket, buffer_to_send, 30, 0, (struct sockaddr *) &clientAddress,
                               clientAddressLength) == -1) {
                        perror("ERROR!");
                        exit(0);
                    }
                }

                // Lock mutex to store content
                pthread_mutex_lock(&mutex);

                token = strtok(NULL, " ");

                // Adds the ID_Cliente to the array of the source
                int i = 0;
                do {
                    if (src[index].clients_id[i] == NULL) {
                        src[index].clients_id[i] = (char *) malloc(2);
                        memcpy(src[index].clients_id[i], token, sizeof(&token));
                        i = num_clients;    // Leave the loop
                    }
                } while (i < num_clients);

                num_clients++;

                // Unlock mutex to store content
                pthread_mutex_unlock(&mutex);


            } else if (strcmp(token, "stop") == 0) {    // If data received is "stop (D)"
                int index = 0;
                token = strtok(NULL, " ");

                // Lock mutex to store content
                pthread_mutex_lock(&mutex);

                // Search for the channel in the array
                for (int i = 0; i < num_sources; i++) {
                    if (strcmp(channel_id[i], token) == 0) {
                        index = i;
                    }
                }

                token = strtok(NULL, " ");

                // Removes the ID_Cliente from the array of the source
                int i = 0;
                do {
                    if (strcmp(src[index].clients_id[i], token) == 0) {
                        free(src[index].clients_id[i]);
                        i = num_clients;    // Leave the loop
                    }
                } while (i < num_clients);

                num_clients--;

                // Unlock mutex to store content
                pthread_mutex_unlock(&mutex);

                // Sends the response to the client
                sprintf(buffer_to_send, "response-stop");
                if (sendto(socket, buffer_to_send, sizeof(buffer_to_send), 0, (struct sockaddr *) &clientAddress,
                           clientAddressLength) == -1) {
                    perror("ERROR!");
                    exit(0);
                }
            }

        }

    }

    pthread_exit(NULL);
}


int main(int argc, char const *argv[]) {

    // Initialize o mutex
    pthread_mutex_init(&mutex, NULL);

    // Create a socket for clients
    int *socket_clients = (int *) malloc(sizeof(int));
    *socket_clients = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (*socket_clients == -1) {
        perror("Failed to create socket_clients");
        exit(EXIT_FAILURE);
    }

    pthread_t source_thread;
    pthread_t client_thread;

    // Create thread for source
    if (pthread_create(&source_thread, NULL, source_Thread, (void *) socket_clients) != 0) {
        perror("Error creating thread");
        return 0;
    }

    // Create thread for clients
    if (pthread_create(&client_thread, NULL, client_Thread, (void *) socket_clients) != 0) {
        perror("Error creating thread");
        return 0;
    }

    pthread_join(source_thread, NULL);
    pthread_join(client_thread, NULL);

    close(*socket_clients);

    // Destroy mutex
    pthread_mutex_destroy(&mutex);

    return 0;
}
