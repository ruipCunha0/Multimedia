#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BLOCK_SIZE 65536
#define MAX_TABLE_SIZE 4096

typedef struct {
    int code;
    int prefix;
    char character;
} Entry;

void compress_block(unsigned char *block, int block_size, FILE *output_file, Entry *dictionary, int *table_size_ptr, int *next_code_ptr, int *prefix_code_ptr) {
    int i, j;
    int current_code;
    char current_char;
    int table_size = *table_size_ptr;
    int next_code = *next_code_ptr;
    int prefix_code = *prefix_code_ptr;

    for (i = 0; i < block_size; i++) {
        current_char = (char)block[i];
        if (dictionary[prefix_code].prefix != -1 && dictionary[prefix_code].character == current_char) {
            prefix_code = dictionary[prefix_code].prefix;
        }
        else {
            if (next_code < MAX_TABLE_SIZE) {
                dictionary[next_code].code = next_code;
                dictionary[next_code].prefix = prefix_code;
                dictionary[next_code].character = current_char;
                next_code++;
                if (next_code > table_size && table_size < MAX_TABLE_SIZE) {
                    table_size *= 2;
                    dictionary = realloc(dictionary, sizeof(Entry) * table_size);
                }
            }
            fwrite(&dictionary[prefix_code].code, sizeof(int), 1, output_file);
            prefix_code = current_char;
        }
    }

    *table_size_ptr = table_size;
    *next_code_ptr = next_code;
    *prefix_code_ptr = prefix_code;
}

void compress(FILE *input_file, FILE *output_file) {
    int i;
    int table_size = 256;
    int next_code = 256;
    int prefix_code;
    unsigned char block[BLOCK_SIZE];
    int bytes_read;
    Entry *dictionary = (Entry*)malloc(sizeof(Entry) * MAX_TABLE_SIZE);

    for (i = 0; i < 256; i++) {
        dictionary[i].code = i;
        dictionary[i].prefix = -1;
        dictionary[i].character = i;
    }
    prefix_code = fgetc(input_file);
    while ((bytes_read = fread(block, sizeof(unsigned char), BLOCK_SIZE, input_file)) > 0) {
        compress_block(block, bytes_read, output_file, dictionary, &table_size, &next_code, &prefix_code);
    }
    fwrite(&dictionary[prefix_code].code, sizeof(int), 1, output_file);
    free(dictionary);
}

int main(int argc, char *argv[]) {
    FILE *input_file;
    FILE *output_file;

    input_file = fopen("example.txt", "rb");
    if (input_file == NULL) {
        printf("Error: Could not open input file.\n");
        return 1;
    }

    output_file = fopen("output.txt", "wb");
    if (output_file == NULL) {
        printf("Error: Could not open output file.\n");
        return 1;
    }

    compress(input_file, output_file);

    fclose(input_file);
    fclose(output_file);

    return 0;
}
