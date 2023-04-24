#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define MAX_DICT_SIZE 4096
#define MAX_INPUT_SIZE 65536

typedef unsigned short code_t; // 16-bit code
typedef unsigned char byte_t; // 8-bit byte


int main(int argc, char *argv[]) {

    // Open input file in binary mode
    FILE *input_file = fopen("example.png", "rb");
    char* buffer;

    // If file could not open return 1
    if (input_file == NULL) {
        printf("Error: could not open input file.\n");
        return 1;
    }

    // Read input file into buffer
    byte_t bytes_buffer[MAX_INPUT_SIZE];
    int file_size = fread(bytes_buffer, sizeof(byte_t), MAX_INPUT_SIZE, input_file); // Stores the content in input buffer
    fclose(input_file);

    // For cycle to print each byte of array
    for (int i = 0; i < file_size; i++) {
        printf("%02x ", bytes_buffer[i] & 0xff);
    }


    return 0;

}
