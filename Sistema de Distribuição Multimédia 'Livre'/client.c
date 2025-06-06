#include <arpa/inet.h>
#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PORT 5005
#define BUFFER_SIZE 1024


// run code: ./client (ID_Cliente)
int main(int argc, char *argv[]) {

    if (argc < 2) {
        perror("ERROR, not enough args!");
        return 1;
    }

    // Receive string from argument
    char *id_string_client = argv[1];

    int client_socket;
    struct sockaddr_in serv_addr;
    socklen_t serv_addr_len = sizeof(serv_addr);
    char buffer[1024];
    char input_string[560];
    char buffer_to_receive[BUFFER_SIZE];
    char *copy;
    char *token;

    int Vi, Fa;

    pid_t pid;

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
                    printf("\n");

                    switch (choice_2) {

                        case 1: // If chosen List
                            sprintf(buffer, "list");

                            // Sends the command to the server
                            sendto(client_socket, buffer, 5, 0, (struct sockaddr *) &serv_addr, serv_addr_len);

                            // Receives the response
                            if(recvfrom(client_socket, buffer_to_receive, BUFFER_SIZE, 0, NULL, 0) < 0) {
                                perror("ERROR!");
                                return -1;
                            }

                            // Separates the string received
                            copy = strdup(buffer_to_receive);
                            strtok(copy, "|");
                            token = strtok(NULL, "|");

                            // Reads all the channels in the server
                            for (size_t i = 0; i < atoi(token); i++) {
                                if(recvfrom(client_socket, buffer_to_receive, BUFFER_SIZE, 0, NULL, 0) < 0) {
                                    perror("ERROR!");
                                    return -1;
                                }

                                printf("%s\n", buffer_to_receive);
                            }


                            break;

                        case 2: // If chosen Info

                            printf("Introduza o ID do canal: ");
                            scanf("%s", input_string);

                            sprintf(buffer, "info %s", input_string);
                            // Sends the command to the server
                            sendto(client_socket, buffer, 15, 0, (struct sockaddr *) &serv_addr, serv_addr_len);

                            // Receives the response
                            if(recvfrom(client_socket, buffer_to_receive, BUFFER_SIZE, 0, NULL, 0) < 0) {
                                perror("ERROR!");
                                return -1;
                            }

                            // Separates the string received
                            copy = strdup(buffer_to_receive);
                            strtok(copy, ",");
                            token = strtok(NULL, ",");

                            // Prints the information
                            printf("%s \n", token);

                            break;

                        case 3: // If chosen PLay

                            printf("Introduza o ID do canal: ");
                            scanf("%s", input_string);

                            sprintf(buffer, "play %s %s", input_string, id_string_client);
                            // Sends the command to the server
                            sendto(client_socket, buffer, 30, 0, (struct sockaddr *) &serv_addr, serv_addr_len);

                            // Receives the response
                            if(recvfrom(client_socket, buffer_to_receive, BUFFER_SIZE, 0, NULL, 0) < 0) {
                                perror("ERROR!");
                                exit(0);
                            }

                            // If channel does not exist do nothing
                            if (strcmp(buffer_to_receive, "channel does not exists...") == 0) {
                                printf("%s \n", buffer_to_receive);

                            } else {

                                pid = fork();

                                if (pid < 0) {
                                    perror("Pid failed!");
                                    exit(0);
                                } else if (pid == 0) {
                                    // Child process

                                    printf("\n");
                                    while (true) {

                                        // Receives the information from the server
                                        if(recvfrom(client_socket, buffer_to_receive, BUFFER_SIZE, 0, NULL, 0) < 0) {
                                            perror("ERROR!");
                                            exit(0);
                                        }

                                        // Separates the string received
                                        copy = strdup(buffer_to_receive);
                                        strtok(copy, " ");

                                        // Check if the id is equal to the client
                                        if (strcmp(copy, id_string_client) == 0) {

                                            // Get the Vi and the Fa from the information
                                            for (size_t i = 0; i < 7; i++) {
                                                token = strtok(NULL, "|");
                                                if (i == 2) {
                                                    Vi = atoi(token);
                                                } else if (i == 6){
                                                    Fa = atoi(token);
                                                }
                                            }

                                            for (size_t i = 0; i < Vi; i++)
                                                printf("-");

                                            printf("\n");
                                            sleep(1 / Fa);

                                        }
                                    }

                                } else {

                                }

                            }

                            break;

                        case 4: // If chosen Stop

                            printf("Introduza o ID do canal: ");
                            scanf("%s", input_string);

                            sprintf(buffer, "stop %s %s", input_string, id_string_client);
                            // Sends the command to the server
                            sendto(client_socket, buffer, 5, 0, (struct sockaddr *) &serv_addr, serv_addr_len);

                            // Receives the response
                            if(recvfrom(client_socket, buffer_to_receive, BUFFER_SIZE, 0, NULL, 0) < 0) {
                                perror("ERROR!");
                                exit(0);
                            }

                            // Compares the string
                            if(strcmp(buffer_to_receive, "response-stop") == 0)
                                printf("%s \n", buffer_to_receive);

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
