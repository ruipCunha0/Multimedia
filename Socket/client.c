#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<fcntl.h>
#include <stdio.h>
#include <string.h>
#include<stdlib.h>

// port through which connection is to be made
#define CONNECTION_PORT 5000


int main() {

    struct sockaddr_in server_address;

    char *message = "This is a message from the client";

    // storage buffer to receive messages
    char recieve_buffer[100];

    // create an Ipv4 and UDP socket
    int socket_descriptor = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_descriptor < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    int status = 0;
    // initialize address structure for binding
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(CONNECTION_PORT);
    server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_address.sin_zero[8] = '\0';

    // connect to the server
    status = connect(socket_descriptor, (struct sockaddr *) &server_address, sizeof(server_address));
    if (status < 0) {
        perror("Couldn't connect with the server");
        exit(EXIT_FAILURE);
    }

    // send message to the server
    write(socket_descriptor, message, strlen(message));

    //receive a message from the server
    read(socket_descriptor, recieve_buffer, 100);

    printf("Message from server: %s\n", recieve_buffer);

    // terminate the socket connection
    close(socket_descriptor);

    return 0;
}
