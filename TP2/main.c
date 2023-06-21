#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>


#define PORT 5000

int main(int argc, char const *argv[]) {

    char buffer[1024];

    // Create a socket
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd == -1) {
        perror("Failed to create socket");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    struct sockaddr_in server_address;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);  // Example port number
    server_address.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the specified address and port
    if (bind(server_fd, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        perror("Failed to bind socket");
        exit(EXIT_FAILURE);
    }

    // Socket starts and listens for connections
    printf("Server listening on port 5000...\n");

    // Read messages from client
    read(server_fd, buffer, 1024);
    printf("%s\n", buffer);

    close(server_fd);

    // closing the listening socket
    shutdown(server_fd, SHUT_RDWR);
    
    return 0;
}
