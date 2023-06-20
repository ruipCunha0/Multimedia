#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PORT 5005


// run code: ./client
int main(int argc, char *argv[]) {


    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024];

    // Create Socket
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server socket
    if ((status = connect(client_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    for (int i = 0; i <= Fa; i++) {
        double t = (double)i / Fa;
        double sample = 1 + (1 + sin(2 * PI * t / N)) * 30;

        sprintf(buffer, "%s|%d|%d|%d|%d|%d|%d|", id_string, i, (int) sample, P, F, N, M);
        send(client_fd, buffer, 25, 0);
    }

    // closing the connected socket
    close(client_fd);
    
    return 0;
}
