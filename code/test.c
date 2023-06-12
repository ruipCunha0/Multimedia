#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>


#define MAX_DICT_SIZE 65536

typedef unsigned char byte_t; // 8-bit byte

typedef struct{
    byte_t *value;
    size_t size;
    bool used;
} lzwd_t;


lzwd_t dictionary[MAX_DICT_SIZE];
int dict_size;




bool equal_bytes_2(lzwd_t map, int index_i) {

    for (int i = 0; i < map.size; i++) {
        if (map.value[i] != dictionary[index_i].value[i]) {
            return false;
        }
    }

    return true;
}

// Function used to create a default dictionary
bool create_dictionary(lzwd_t dictionary[]) {
    dict_size = 256;

    for (int i = 0; i < dict_size ; i++ ) {        // From 0 to 255
        dictionary[i].value = (byte_t*) malloc(sizeof (byte_t));
        if(dictionary[i].value == NULL) {
            return false;
        }
        dictionary[i].value[0] = i;      // Position 0 will be it's own index
        dictionary[i].size = 1;
        dictionary[i].used = false;
    }

    // All remaining indexs will be 0
    for (int i = dict_size; i < MAX_DICT_SIZE ; i++ ) {      // From 256 to 65536
        dictionary[i].value = NULL;
        dictionary[i].size = 0;
        dictionary[i].used = false;
    }

    return true;
}



// Function used to verify if the bytes of the two arrays is equal
bool equal_bytes(lzwd_t map, byte_t pattern, size_t size_of_pattern, int index_i) {
    map.value[size_of_pattern - 1] = pattern;

    for (int i = 0; i < map.size; i++) {
        if (map.value[i] != dictionary[index_i].value[i]) {
            return false;
        }
    }

    return true;
}



// Function used to find the index of the pattern
int find_dictionary_sequence(lzwd_t map, byte_t pattern, size_t size_of_pattern) {

    for (int i = 0; i < dict_size; i++) {
        if (dictionary[i].size == size_of_pattern) {
            if (equal_bytes(map, pattern, size_of_pattern, i)) {
                return i;
            }
        }
    }

    return -1;

}


int search_dictionary_pattern(lzwd_t Pa, lzwd_t Pb) {
    lzwd_t temp_pattern;
    temp_pattern.value = (byte_t*) malloc(sizeof (byte_t));
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

    printf("\n\n\n");
    for (int i = 0; i < dict_size; i++) {
        for (int j = 0; j < temp_pattern.size; j++) {
            if (equal_bytes_2(temp_pattern, i)) {
                return i;
            }
        }
    }

    return -1;
}


int main(int argc, char** argv) {

    FILE* input_fp = fopen("example.txt", "rb");
    if (input_fp == NULL) {
        printf("Error: could not open input file '%s'\n", argv[1]);
        return 1;
    }

    FILE* output_fp = fopen("output.txt", "wb");
    if (output_fp == NULL) {
        printf("Error: could not open output file '%s'\n", argv[2]);
        return 1;
    }

    // Creates the default dictionary
    if(!create_dictionary(dictionary)) {
        return 0;
    }

    size_t block_size;
    char block[MAX_DICT_SIZE];
    int dict_index = 0;

     while ((block_size = fread(block, sizeof(char), MAX_DICT_SIZE, input_fp)) > 0) {

        for (int i = 0; i < 10; i++) {
            // printf("%d ", block[i] & 0xff);
        }

        // Initializate a counter to work as an index of the position in the block array
        int counter = 0;
        int counter_index = 0;

        // Initializate Pa with the first byte of the block
        lzwd_t Pa;
        Pa.value = (byte_t*) malloc(sizeof (byte_t));
        Pa.value[0] = block[counter];
        Pa.size = 1;

        counter++;

        // while counter < block_size(65536)
        while(counter < 3) {

            // Creates a Pb variable and allocates memory to it
            lzwd_t Pb;

            Pb.value = (byte_t*) malloc(sizeof (byte_t));
            Pb.size = 1;
            Pb.value[0] = block[counter];

            counter_index = counter;
            int j = 1;

            while(counter_index + j < block_size) {
                if (find_dictionary_sequence(Pb, block[counter_index + j], Pb.size + 1) != -1) {
                    // printf("HEREEE!");
                    j++;
                    Pb.value[Pb.size] = block[counter_index + j];
                    Pb.size = Pb.size + 1;
                }
                counter_index++;
            }

            dict_index = search_dictionary_pattern(Pa, Pb);

            if (dict_index != -1) {

                printf("\n Sequence found for %d! Index: %d", Pa, Pb);

            } else {

                printf("\n %d", dict_index);

            counter++;
            Pa = Pb;

            }
        }


     }


    return 0;

}
