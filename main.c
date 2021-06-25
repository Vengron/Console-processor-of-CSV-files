#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "BST.h"

#define CLI_INPUT_LEN 64
#define DELIMITERS ";"

int detect_line_breaks(char *text) {
    int d = (int) strlen(text);

    if (text[d - 2] == '\r') {
        return 2;
    } else if (text[d - 1] == '\n') {
        return 1;
    }

    return 0;
}

// loads list of states from file_name and creates BST from it
Country *load_file(char *file_name, int type_of_key) {
    FILE *in;
    if (!(in = fopen(file_name, "r"))) {
        printf("Soubor %s se nepodařilo načíst.\n", file_name);
        exit(-1);
    }

    Country *root = NULL, *curr_node;
    char *key, *names[NAMES_NUM], *curr_row;
    long buffer_size = 0;
    int line_break;

    getline(&curr_row, (size_t *) &buffer_size, in);
    line_break = detect_line_breaks(curr_row);

    while (getline(&curr_row, (size_t *) &buffer_size, in) != -1) {
        curr_row[strlen(curr_row) - line_break] = '\0';

        // loads desired key
        key = strtok(curr_row, DELIMITERS);
        for (int i = 0; i < type_of_key; ++i) {
            key = strtok(NULL, DELIMITERS);
        }
        // skips rest of keys
        for (int i = type_of_key + 1; i < KEYS_NUM; ++i) {
            strtok(NULL, DELIMITERS);
        }
        // loads all names
        for (int i = 0; i < NAMES_NUM; ++i) {
            names[i] = strtok(NULL, DELIMITERS);
            //printf("%s\n", names[i]);
        }
        // adds node/root
        curr_node = add_node(root, key, names);
        if (!curr_node) {
            printf("Stát s kódem %s už byl načten.\n", key);
        }
        root = !root ? curr_node : root;
    }

    fclose(in);
    return root;
}


void print_all_countries(Country *node) {
    if (!node) {
        return;
    }
    print_all_countries(node->left);
    printf("%s; ", node->key);
    for (int i = 0; i < NAMES_NUM; ++i) {
        printf("%s; ", node->names[i]);
    }
    printf("\n");
    print_all_countries(node->right);
}


void find_and_print_country(Country *root) {
    char input[CLI_INPUT_LEN];
    Country *node;

    printf("Zadejte klíč státu:\n");
    fgets(input, CLI_INPUT_LEN, stdin);
    input[strlen(input) - detect_line_breaks(input)] = '\0';

    node = search_tree(root, input);
    if (node) {
        printf("%s; ", node->key);
        for (int i = 0; i < NAMES_NUM; ++i) {
            printf("%s; ", node->names[i]);
        }
        printf("\n");
    } else {
        printf("Stát s daným klíčem nenalezen.\n");
    }
}

// ask user to choose type of key and then call load_file()
Country *choose_key(char *file_name) {
    char input[CLI_INPUT_LEN];
    int type_of_key;
    Country *root;

    printf("Který typ kódu chcete použít:\n"
           "0 - číselný\n1 - dvoumístný\n2 - třímístný\n");
    fgets(input, CLI_INPUT_LEN, stdin);
    type_of_key = (int) strtol(input, NULL, 10);
    if (type_of_key < 0 || 2 < type_of_key) {
        printf("Typ klíče je třeba vybrat pomocí 0, 1 nebo 2.\n");
        exit(-1);
    }

    root = load_file(file_name, type_of_key);
    printf("Načteno %d států.\n", count_nodes(root));
    printf("Počet vrstev BVS je %d.\n", count_layers(root));

    return root;
}


int main() {
    char input[CLI_INPUT_LEN], file_name[CLI_INPUT_LEN];
    Country *root;

    printf("Zadejte soubor k zpracování:\n");
    fgets(file_name, CLI_INPUT_LEN, stdin);
    file_name[strlen(file_name) - detect_line_breaks(file_name)] = '\0';

    root = choose_key(file_name);

    while (1) {
        printf("\nVyberte požadovanou akci:\n"
               "p - vypsat seznam států seřazený dle klíče\n"
               "f - najít stát dle zadaného klíče\n"
               "c - změnit klíč\n"
               "q - ukončit program\n");
        fgets(input, CLI_INPUT_LEN, stdin);

        switch (input[0]) {
            case 'p':
                print_all_countries(root);
                break;
            case 'f':
                find_and_print_country(root);
                break;
            case 'c':
                delete_tree(root);
                root = choose_key(file_name);
                break;
            case 'q':
                delete_tree(root);
                printf("Program korektně ukončen.\n");
                return 0;
            default:
                printf("Neplatný příkaz.\n");
                break;
        }
    }
}
