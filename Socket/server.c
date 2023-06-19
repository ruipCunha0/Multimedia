#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

// port through which connection is to be made
#define CONNECTION_PORT 5000

int main() {

    int socket_descriptor;
    int client_socket;

    // buffer to store message
    char storage_buffer[80];
    int length_of_address;

    int option_value = 1;

    struct sockaddr_in server_address;
    struct sockaddr_in connection_address;
    char *message = "This is a message from the server";

    // creating the socket with IPv4 domain and UDP protocol
    socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_descriptor < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // set options for the socket
    int status = setsockopt(socket_descriptor, SOL_SOCKET, SO_REUSEADDR, &option_value, sizeof(option_value));
    if (status < 0) {
        perror("Couldn't set options");
        exit(EXIT_FAILURE);
    }

    // initializing structure elements for address
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONNECTION_PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_zero[8] = '\0';

    // bind the socket with the values address and port from the sockaddr_in structure
    status = bind(socket_descriptor, (struct sockaddr *) &server_address, sizeof(struct sockaddr));
    if (status < 0) {
        perror("Couldn't bind socket");
        exit(EXIT_FAILURE);
    }

    // listen on specified port with a maximum of 4 requests
    status = listen(socket_descriptor, 4);
    if (status < 0) {
        perror("Couldn't listen for connections");
        exit(EXIT_FAILURE);
    }

    length_of_address = sizeof(connection_address);
    // accept connection signals from the client
    client_socket = accept(socket_descriptor, (struct sockaddr *) &connection_address, &length_of_address);
    if (client_socket < 0) {
        perror("Couldn't establish connection with client");
        exit(EXIT_FAILURE);
    }

    // Receive data sent by the client
    read(client_socket, storage_buffer, 80);
    // set the last index of the character array as a null character
    storage_buffer[bytes_read] = '\0';

    printf("Message from client: %s \n", storage_buffer);

    // Send data to the client
    send(client_socket, message, strlen(message), 0);

    // Close all the sockets created
    close(socket_descriptor);
    close(client_socket);

    return 0;
}
