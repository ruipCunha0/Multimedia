#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define BLOCK_SIZE 65536
#define MAX_DICT_SIZE 65536

typedef struct {
    int code;
    int prefix;
    char character;
} Dict;


void compress(FILE *input_file, FILE *output_file) {

    unsigned char block[BLOCK_SIZE];
    int bytes_read;
    char current_character;
    Dict* dictionary = (Dict*)malloc(sizeof(Dict) * MAX_DICT_SIZE);

    // initializing dictionary
    for (int i = 0; i < 256; i++) {
        dictionary[i].code = i;
        dictionary[i].prefix = -1;
        dictionary[i].character = i;
    }


    // read the file in blocks of 64 Kbytes
    while ((bytes_read = fread(block, sizeof(unsigned char), BLOCK_SIZE, input_file)) > 0) {

        for (int i = 0; i < bytes_read; i++) {
            current_character = (char)block[i];
            printf("")
        }
    }
}

int main(int argc, char *argv[]) {
    FILE *input_file;
    FILE *output_file;

    input_file = fopen("example2.png", "rb");
    if (input_file == NULL) {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    output_file = fopen("output.png", "wb");
    if (output_file == NULL) {
        printf("Error: Could not open output file.\n");
        return 1;
    }

    compress(input_file, output_file);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
