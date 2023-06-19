#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 5000

int main(int argc, char const *argv[]) {

    int new_socket, valread;
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


    if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    // Read messages from client
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);

    // Sends messages to client
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    // closing the connected socket
    close(new_socket);
    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    
    return 0;
}
