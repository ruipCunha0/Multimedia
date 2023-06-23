#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <string.h>

#define MAX_DICT_SIZE 65536
#define MAX_BLOCK_SIZE 65536

typedef unsigned char byte_t; // 8-bit byte

typedef struct {
    byte_t *value;
    size_t size;
} lzwd_t;


lzwd_t dictionary[MAX_DICT_SIZE];
size_t dict_size;


// Function to print the information about the code
void print_information() {
    printf("Rui Pedro Cunha, a93093 \n");
    printf("Francisco Martins, a93079 \n");
    printf("15/06/2023 \n\n");
    printf("Dictionary maximum size: %d bytes \n", MAX_DICT_SIZE);
}


bool equal_bytes(lzwd_t map, int index_i) {

    for (int i = 0; i < map.size; i++) {
        if (map.value[i] != dictionary[index_i].value[i]) {
            return false;
        }
    }
    return true;
}

// Function used to create a default dictionary
bool create_dictionary(lzwd_t dict[]) {
    dict_size = 256;

    for (int i = 0; i < dict_size; i++) {        // From 0 to 255
        dict[i].value = (byte_t *) malloc(sizeof(byte_t));
        if (dict[i].value == NULL) {
            return false;
        }
        dict[i].value[0] = i;      // Position 0 will be its own index
        dict[i].size = 1;
    }

    // All remaining indexes will be 0
    for (size_t i = dict_size; i < MAX_DICT_SIZE; i++) {      // From 256 to 65536
        dict[i].value = NULL;
        dict[i].size = 0;
    }

    return true;
}


// Function used to find the index of the pattern
int find_dictionary_sequence(lzwd_t map, byte_t pattern, size_t size_of_pattern) {
    map.value[size_of_pattern - 1] = pattern;
    map.size = size_of_pattern;

    for (int i = 0; i < dict_size; i++) {
        if (dictionary[i].size == size_of_pattern) {
            if (equal_bytes(map, i)) {
                return i;
            }
        }
    }

    return -1;

}


// Search in the dictionary if the pattern Pa + Pb exists
int search_dictionary_pattern(lzwd_t Pa, lzwd_t Pb) {
    lzwd_t temp_pattern;
    temp_pattern.value = (byte_t *) malloc(sizeof(byte_t) * (Pa.size + Pb.size));
    if (temp_pattern.value == NULL) {
        printf("\nError! memory not allocated.");
        exit(0);
    }
    temp_pattern.size = Pa.size + Pb.size;

    int i = 0;

    for (int i_1 = 0; i_1 < Pa.size; i_1++) {
        temp_pattern.value[i] = Pa.value[i_1];
        i++;
    }
    for (int i_2 = 0; i_2 < Pb.size; i_2++) {
        temp_pattern.value[i] = Pb.value[i_2];
        i++;
    }

    for (int x = 0; x < dict_size; x++) {
        for (int j = 0; j < temp_pattern.size; j++) {
            if (equal_bytes(temp_pattern, x)) {
                free(temp_pattern.value);
                return x;
            }
        }
    }

    return -1;
}


// Function used to free the memory of the dictionary
void free_map(lzwd_t map[]) {
    for (size_t i = 0; i < MAX_DICT_SIZE; i++) {
        if (map[i].value == NULL) {
            break;
        }
        free(map[i].value);
    }
}


// iterate over the dictionary to find the ID of the Pa pattern
int find_Pa_code(lzwd_t Pa) {

    for (int i = 0; i <= dict_size; i++) {
        if (equal_bytes(Pa, i)) {
            return i;
        }
    }
    return -1;
}


int main(__attribute__((unused)) int argc, char **argv) {

    print_information();
    clock_t time_start = clock();


    FILE *input_fp = fopen("/home/rui/Desktop/Multimedia/C/files/pdf.pdf", "rb");
    if (input_fp == NULL) {
        printf("Error: could not open input file '%s'\n", argv[1]);
        return 1;
    }

    FILE *output_fp = fopen("/home/rui/Desktop/Multimedia/C/output.txt.lzwd", "wb");
    if (output_fp == NULL) {
        printf("Error: could not open output file '%s'\n", argv[2]);
        return 1;
    }

    // Creates the default dictionary
    if (!create_dictionary(dictionary)) {
        return 0;
    }

    size_t block_size;
    size_t block_size_total = 0;
    size_t number_of_blocks = 0;
    size_t dict_reset = 0;
    size_t number_of_outputs = 0;

    // Initialize a counter to work as an index of the position in the block array
    size_t counter;
    char block[MAX_BLOCK_SIZE];
    char *copy;
    int code_output;

    lzwd_t Pa;
    lzwd_t Pb;


    while ((block_size = fread(block, sizeof(char), MAX_BLOCK_SIZE, input_fp)) > 0) {
        printf("\n %zu", block_size);
        number_of_blocks++;
        block_size_total += block_size;

        // Initialize a counter to work as an index of the position in the block array
        counter = 0;

        // Initialize Pa with the first byte of the block
        Pa.value = (byte_t *) malloc(sizeof(byte_t));
        if (Pa.value == NULL) {
            printf("\nError! memory not allocated.");
            exit(0);
        }
        Pa.value[0] = block[counter];
        Pa.size = 1;

        counter++;

        // while counter < block_size(65536)
        while (counter < block_size) {

            // Creates a Pb variable and allocates memory to it
            Pb.value = (byte_t *) malloc(sizeof(byte_t));
            if (Pb.value == NULL) {
                printf("\nError! memory not allocated.");
                exit(0);
            }

            Pb.size = 1;
            Pb.value[0] = block[counter];

            int j = 1;

            while (counter + j < block_size) {
                if (find_dictionary_sequence(Pb, block[counter + j], Pb.size + 1) != -1) {
                    Pb.value[Pb.size] = block[counter + j];
                    Pb.size = Pb.size + 1;

                    j++;
                } else {
                    break;
                }
            }

            /*
            printf("\n Biggest pattern known: ");
            for (int i = 0; i < Pb.size; i++) {
                printf("%d", Pb.value[i]);
            }
            */

            // Search for the Pa code to write in the output
            code_output = find_Pa_code(Pa);
            if (code_output == -1) {
                printf("\n ERROR!!! %zu", counter);
                return 0;
            }

            // Write the code to the output file
            fwrite(&code_output, sizeof(code_output), 1, output_fp);
            fwrite(",", 1, 1, output_fp);
            number_of_outputs++;

            printf("\n Counter: %zu and dic_size: %zu", counter, dict_size);


            int index = 1;

            while (index <= j) {

                // Add the pattern in dictionary
                if (dict_size == MAX_DICT_SIZE) {

                    dict_reset++;

                    free_map(dictionary);
                    // Creates the default dictionary
                    if (!create_dictionary(dictionary)) {
                        return 0;
                    }

                }

                Pb.size = index;
                if ((search_dictionary_pattern(Pa, Pb) == -1)) {

                    dictionary[dict_size].value = (byte_t *) malloc(sizeof(byte_t));
                    if (dictionary[dict_size].value == NULL) {
                        printf("\nError! memory not allocated.");
                        exit(0);
                    }
                    dictionary[dict_size].size = Pa.size + Pb.size;

                    int i = 0;

                    for (int i_1 = 0; i_1 < Pa.size; i_1++) {
                        dictionary[dict_size].value[i] = Pa.value[i_1];
                        i++;
                    }
                    for (int i_2 = 0; i_2 < Pb.size; i_2++) {
                        dictionary[dict_size].value[i] = Pb.value[i_2];
                        i++;
                    }

                    /*
                    printf("\n Pattern added to dictionary: ");
                    for (int ind = 0; ind < dictionary[dict_size].size; ind++) {
                        printf("%d", dictionary[dict_size].value[ind]);
                    }
                    */

                    dict_size++;

                }

                index++;

            }


            counter += Pb.size;
            copy = strdup(Pb.value);
            memcpy(&Pa.value, &copy, sizeof(copy));

            free(Pa.value);
            free(Pb.value);
        }

    }


    // Search for the last Pa code to write in the output
    code_output = find_Pa_code(Pa);
    if (code_output == -1) {
        printf("\n ERROR!!! %zu", counter);
        return 0;
    }

    // Write the last code to the output file
    fwrite(&code_output, sizeof(code_output), 1, output_fp);
    number_of_outputs++;

    free_map(dictionary);
    fclose(input_fp);
    fclose(output_fp);

    clock_t time = clock() - time_start;
    double time_taken = ((double) time) / CLOCKS_PER_SEC; // calculate the elapsed time

    printf("\n Compression successful!");
    printf("\n ----------------------------------------");
    printf("\n Size of block: %zu", block_size_total);
    printf("\n ----------------------------------------");
    printf("\n Number of blocks: %zu", number_of_blocks);
    printf("\n ----------------------------------------");
    printf("\n Dictionary resets: %zu", dict_reset);
    printf("\n ----------------------------------------");
    printf("\n Size of dictionary: %zu", dict_size);
    printf("\n ----------------------------------------");
    printf("\n Number of outputs: %zu", number_of_outputs);
    printf("\n ----------------------------------------");
    printf("\n Time elapsed: %.2f seconds", time_taken);
    printf("\n ----------------------------------------");

    return 0;

}
