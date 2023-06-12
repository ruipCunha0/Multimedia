#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DICT_SIZE 65536

typedef unsigned char byte_t; // 8-bit byte

//node structure
struct node{
    byte_t data;
    struct node *next;
};

struct node *dictionary[MAX_DICT_SIZE];
struct node *temp;
int dict_size;

// Function used to create a default dictionary
void create_dictionary() {
    dict_size = 256;
    for (int i = 0; i < 256; i++) {
        dictionary[i] = malloc(sizeof(struct node));
        dictionary[i]->data = i;
        dictionary[i]->next = NULL;
    }
}


// Function used to find the index of the pattern
int find_dictionary_entry(byte_t Pa, byte_t Pb) {
    for (int i = 0; i < dict_size; i++) {
        if (dictionary[i].prefix == Pa && dictionary[i].symbol == Pb) {
            return i;
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

    // initializate the dictionary with the default bytes
    create_dictionary();

    char block[MAX_DICT_SIZE];
    size_t block_size;
    int dict_index = 0;


    while ((block_size = fread(block, sizeof(char), MAX_DICT_SIZE, input_fp)) > 0) {
        for (int i = 0; i < 50; i++) {
            printf("%d ", block[i] & 0xff);
        }

        int counter = 0;
        int code_output = 0;
        int counter_index = 0;

        // Initializate Pa with the first byte of the block
        byte_t Pa = block[counter];
        counter++;

        while (counter < 10) {

            byte_t Pb = block[counter];
            byte_t temp_Pb = Pb;
            // printf("\nNext Symbol: %d Counter: %d", next_symbol, counter);

            counter_index = counter;
            int j = 1;

            while(counter_index + j < block_size) {

                temp->data = Pb;
                temp->next
            }

            printf("\n Biggest pattern: %d and dict size: %d", temp_Pb, dict_size);


            dict_index = find_dictionary_entry(Pa, Pb);
            if (dict_index != -1) {

                printf("\n Sequence found for %d! Index: %d", Pa, Pb);

            } else {

                // printf("\n No sequence found for %d%d!", Pa, Pb);

                // Add the pattern in dictionary
                if (dict_size < MAX_DICT_SIZE) {
                    dictionary[dict_size].prefix = Pa;
                    dictionary[dict_size].symbol = Pb;
                    dict_size++;

                    // printf("\n Added to dictionary, Dict size: %d", dict_size - 1);

                    for (int i = 0; i < dict_size; i++) {
                        if (Pa == dictionary[i].code) {
                            code_output = dictionary[i].code;
                        }
                    }

                    code_output = get_index_of_Pa(Pa);
                    printf("\n Code output: %d\n", code_output);
                    // Write the prefix code to the output file
                    fwrite(&code_output, sizeof(int), 1, output_fp);
                }
            }


            Pa = Pb;
            counter++;

        }


    }


    return 0;

}
