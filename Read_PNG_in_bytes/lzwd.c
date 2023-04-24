#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define MAX_DICT_SIZE 65536 // Max. dictionary size -> 2^(16) = 65536
#define ALHPABET_SIZE 256 // Alphabet size -> 2⁸ = 256
#define BLOCK_SIZE (sizeof(char) * 8) // Each block as a capacity of 8 bytes

void lzw_compress(char *input_file, char *output_file) {
    FILE *input = fopen(input_file, "r"); // Open input file for reading
    FILE *output = fopen(output_file, "w"); // Open output file for writting

    if (input == NULL || output == NULL) {
        printf("Erro na abertura dos ficheiros.\n");
        _exit(1);
    }

    unsigned int dict_size = 256; // Max. dictionary size 
    unsigned int dict[MAX_DICT_SIZE];

    // Initialization of the dictionary
    for (unsigned int i = 0; i < MAX_DICT_SIZE; i++) 
        dict[i] = 0;
    
    char* buffer =  (char*) malloc(BLOCK_SIZE);
    unsigned int dict_index;
    unsigned int block_size;
    unsigned int last_code = 0;
    int num_blocks = 0;

    clock_t start_time = clock();

    while ((block_size = fread(buffer, 1, BLOCK_SIZE, input)) > 0) { // Read if there's bytes available 
        dict_index = 256;
        unsigned int *block = malloc((block_size / 2) * sizeof(unsigned int));
        unsigned int block_index = 0;

        for (unsigned int i = 0; i < block_size; i += 2) {
            unsigned int code = ((unsigned int)buffer[i]) << 8 | buffer[i+1];
            unsigned int pattern = last_code << 8 | buffer[i];
            unsigned int next_pattern = last_code << 8 | buffer[i+1];

            unsigned int j = 0;
            while (j < dict_size && dict[j] != pattern) {
                j++;
            }

            if (j < dict_size) {
                last_code = j;
            } else {
                if (dict_size < MAX_DICT_SIZE) {
                    dict[dict_size++] = pattern;
                }
                block[block_index++] = last_code;
                last_code = buffer[i];
            }

            if (j == dict_size - 1 && dict_size < MAX_DICT_SIZE) {
                dict[dict_size++] = next_pattern;
            }
        }

        if (last_code != 0) {
            block[block_index++] = last_code;
        }

        fwrite(block, sizeof(unsigned int), block_index, output);
        free(block);
    } 
    // EOF reached
    fclose(input);
    fclose(output);
    free(buffer);

    // print_stats(start_time, clock(), ftell(input), num_blocks);
}

int main() {
    lzw_compress("sequence.txt", "code.txt");
}