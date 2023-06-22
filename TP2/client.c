#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define PORT 5005
#define BUFFER_SIZE 1024


// run code: ./client
int main(int argc, char *argv[]) {

    int client_socket;
    int status;
    int valread;
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);
    char buffer[1024];
    char input_string[560];
    char buffer_to_receive[BUFFER_SIZE];

    while (1) {

        printf("1. Conectar ao servidor\n");
        printf("2. Sair \n");
        printf("Escolha uma opção: ");

        int choice;
        scanf("%d", &choice);

        switch (choice) {

            case 1:
                // Create Socket
                if ((client_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
                    printf("\n Socket creation error \n");
                    return -1;
                }

                serv_addr.sin_family = AF_INET;
                serv_addr.sin_port = htons(PORT);
                if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
                    printf("\nInvalid address/ Address not supported \n");
                    return -1;
                }

                socklen_t *len;

                while (1) {

                    printf("\n1. list \n");
                    printf("2. info (D) \n");
                    printf("3. play (D) \n");
                    printf("4. stop (D) \n");
                    printf("5. Sair \n");
                    printf("Escolha uma opção: ");

                    int choice_2;
                    scanf("%d", &choice_2);

                    switch (choice_2) {

                        case 1:
                            sprintf(buffer, "list");

                            sendto(client_socket, buffer, 5, 0, (struct sockaddr *) &serv_addr, serv_addr_len);

                            if(recvfrom(client_socket, buffer_to_receive, BUFFER_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_len) < 0) {
                                perror("ERROR!");
                                return -1;
                            }

                            char *copy = strdup(buffer_to_receive);
                            char *token = strtok(copy, "|");
                            token = strtok(NULL, "|");

                            for (size_t i = 0; i < atoi(token); i++) {
                                if(recvfrom(client_socket, buffer_to_receive, BUFFER_SIZE, 0, (struct sockaddr*)&serv_addr, &serv_addr_len) < 0) {
                                    perror("ERROR!");
                                    return -1;
                                }

                                printf("%s\n", buffer_to_receive);
                            }


                            break;

                        case 2:

                            printf("Introduza o ID do canal: ");
                            scanf("%s", input_string);

                            printf("O canal escolhido é: %s\n", input_string);

                            sprintf(buffer, "info %s", input_string);
                            send(client_socket, buffer, 5 + strlen(input_string), 0);

                            break;

                        case 3:

                            printf("Introduza o ID do canal: ");
                            scanf("%s", input_string);

                            printf("O canal escolhido é: %s\n", input_string);

                            sprintf(buffer, "play %s", input_string);
                            send(client_socket, buffer, 5 + strlen(input_string), 0);

                            break;

                        case 4:

                            printf("Introduza o ID do canal: ");
                            scanf("%s", input_string);

                            printf("O canal escolhido é: %s\n", input_string);

                            sprintf(buffer, "stop %s", input_string);
                            send(client_socket, buffer, 5 + strlen(input_string), 0);

                            break;

                        case 5:
                            printf("Saindo... \n");
                            exit(0);

                        default:

                            printf("Opção não disponível...");
                            break;

                    }
                }

            case 2:
                printf("Saindo... \n");

                // closing the connected socket
                close(client_socket);

                exit(0);

            default:

                printf("Opção não disponível...");

        }

    }

}
