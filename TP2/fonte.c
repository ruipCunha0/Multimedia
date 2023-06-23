#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define PI 3.14159265


// run code: ./fonte 2000 (ID)
int main(int argc, char *argv[]) {

    if (argc < 3) {
        perror("ERROR, not enough args!");
        return 1;
    }

    int port = atoi(argv[1]);
    char *id_string = argv[2];

    int F;
    int N;

    // Get F value
    printf("Set number for F: ");
    scanf("%d", &F);
    if (F <= 0) {
        perror("ERROR!");
        return 1;
    }

    // Get N value
    printf("Set number for N, N > 3: ");
    scanf("%d", &N);
    if (N <= 3) {
        perror("ERROR!");
        return 1;
    }

    int Fa = F * N; // Calculate the sampled frequency

    int status, valread, client_fd;
    struct sockaddr_in serv_addr;
    char buffer[1024];
    srand((unsigned) time(NULL));

    // Create Socket
    if ((client_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // Connect to server socket
    if ((status = connect(client_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    int index = 0;
    int P = 0;



    while (true) {
        P = 1;
        if (index == 0) {
            for (int counter = 1; counter <= Fa; counter++) {
                int i = (rand() % Fa) + 1;

                sprintf(buffer, "%s|%d|%d|%d|%d|%d|%d|", id_string, i, 0, P++, F, N, Fa);
                send(client_fd, buffer, 25, 0);

                sleep((1 / Fa));
            }
        } else {
            for (int counter = 1; counter <= Fa; counter++) {
                double t = (double) counter / Fa;
                int i = (rand() % Fa) + 1;
                double sample = 1 + (1 + sin(2 * PI * t / N)) * 30;

                sprintf(buffer, "%s|%d|%d|%d|%d|%d|%d|", id_string, i, (int) sample, P++, F, N, Fa);
                send(client_fd, buffer, 25, 0);

                sleep((1 / Fa));
            }
        }
        index++;
    }


    // closing the connected socket
    close(client_fd);

    return 0;
}
