#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<math.h>

#define MAX_DICTIONARY_SIZE 65536
#define MAX_INPUT_SIZE 65536

typedef unsigned short code_t; // 16-bit code
typedef unsigned char byte_t; // 8-bit byte

struct dictionary{
    size_t size;    // size in bytes of pattern
    int index;  // index of pattern
    byte_t pattern; // pattern
};

FILE *open_file() {

    // Open input file in binary mode
    FILE *fp = fopen("example.png", "rb");

    // If file could not open return 1
    if (fp == NULL) {
        printf("Error: could not open input file.\n");
        return 0;
    }

    return fp;
}

// Initialize dictionary with single-character entries
void init_dictionary(struct dictionary dictionary[MAX_DICTIONARY_SIZE]) {
    for (int i = 0; i < 256; i++) {
        dictionary[i].index = i;
        dictionary[i].pattern = i;
        dictionary[i].size = (dictionary[i].pattern==0)?1:log10(dictionary[i].pattern) + 1; // Count the number of digits in ints;
    }
}


// Add the new pattern to the struct dictionary
void add_to_dictionary(struct dictionary dictionary[MAX_DICTIONARY_SIZE], int pattern_to_add, int dictionary_size) {

    dictionary[dictionary_size].pattern = pattern_to_add;
    dictionary[dictionary_size].index = dictionary_size;

}

// Find the pattern in the dictionary and returns it's index
int find_pattern_in_dictionary(struct dictionary dictionary[MAX_DICTIONARY_SIZE], int pattern, int dictionary_size) {

    for (int i = 0; i < dictionary_size; i++) {
        if (dictionary[i].pattern == pattern) {

            return i;
        }
    }

    return -1;
}


// Find the index of the dictionary entry with the given prefix code and suffix character
int find_seq(struct dictionary dictionary[MAX_DICTIONARY_SIZE], byte_t Pa, byte_t Pb, int dictionary_size) {

    for (int i = 0; i < dictionary_size; i++) {
        if (dictionary[i].pattern == Pa) {
            return i;
        }
    }

    return -1; // entry not found
}

// Function to concatenate the integers and returns the resulting integer
int concatenate(byte_t Pa, byte_t Pb) {

    char string_res[50];
    sprintf(string_res, "%d%d", Pa, Pb);
    int res = strtol(string_res, NULL, 10);

    return res;
}



int main(int argc, char *argv[]) {

    char* buffer;
    FILE *input_file = open_file();

    // Initialize dictionary with single-character entries
    struct dictionary dictionary[MAX_DICTIONARY_SIZE];
    init_dictionary(dictionary);
    int dictionary_size = 256; // number of entries in dictionary

    // Read input file into buffer
    byte_t bytes_buffer[MAX_INPUT_SIZE];
    int file_size = fread(bytes_buffer, sizeof(byte_t), MAX_INPUT_SIZE, input_file); // Stores the content in input buffer
    fclose(input_file);

    // For cycle to print each byte of array
    for (int i = 0; i < 50; i++) {
        printf("%d ", bytes_buffer[i] & 0xff);
    }

    // Open output file
    FILE *output_file = fopen("output.png", "wb");
    if (output_file == NULL) {
        printf("Error: could not open output file.\n");
        return 1;
    }



    // Initialize current code with '0' and prefix code with first byte of input buffer
    code_t current_code = 0;
    byte_t Pa = bytes_buffer[0];

    for (int i = 1; i < 10; i++) {
        byte_t Pb = bytes_buffer[i];

        int pattern_temp = concatenate(Pb, bytes_buffer[i+1]);
        int index_res = find_pattern_in_dictionary(dictionary, pattern_temp, dictionary_size);

        if (index_res != -1) {
            printf("\n Sequence found for %d! Index: %d", pattern_temp, index_res);

        } else {

            printf("\n No sequence found for %d!", pattern_temp);

            int pattern = concatenate(Pa, Pb);
            printf("\nPa: %d ; Pb: %d -- Pattern: %d ", Pa, Pb, pattern);

            int index = find_pattern_in_dictionary(dictionary, pattern, dictionary_size);
            if (index != -1) {
            printf("\nPattern found");

            } else {

                if (dictionary_size == MAX_DICTIONARY_SIZE) {
                    // Dictionary is full, cannot add more entries
                    printf("Error: dictionary is full.\n");
                    return 1;
                }

                add_to_dictionary(dictionary, pattern, dictionary_size);
                printf("\n Added to dictionary");
                current_code = dictionary_size++;
                Pa = Pb;

                // Write the prefix code to the output file
                fwrite(&current_code, sizeof(code_t), 1, output_file);
            }

        }

    }

}
