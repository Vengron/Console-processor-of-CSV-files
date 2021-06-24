#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define CLI_INPUT_LEN 64
#define NAMES_NUM 3
#define KEYS_NUM 3
#define DELIMITERS ";"

// Structure and functions for BST
struct Ctr {
    char *key;
    char *names[NAMES_NUM];
    struct Ctr *left;
    struct Ctr *right;
};
typedef struct Ctr Country;

Country *create_node(char *key, char *names[NAMES_NUM]) {
    Country *country = (Country *) malloc(sizeof(Country));
    if (!country) {
        return NULL;
    }
    char *c_key = (char *) malloc(sizeof(char) * (strlen(key) + 1));
    strcpy(c_key, key);
    country->key = c_key;

    for (int i = 0; i < NAMES_NUM; ++i) {
        char *c_name = (char *) malloc(sizeof(char) * (strlen(names[i]) + 1));
        strcpy(c_name, names[i]);
        country->names[i] = c_name;
    }

    country->left = NULL;
    country->right = NULL;

    return country;
}

// help function for sorting the keys based on the data types
int compare_keys(char *old_key, char *new_key) {
    if (isdigit(old_key[0])) {
        long old, new;
        old = strtol(old_key, NULL, 10);
        new = strtol(new_key, NULL, 10);
        if (old > new) {
            return 1;
        } else if (old < new) {
            return -1;
        }
    } else {
        if (strcmp(old_key, new_key) > 0) {
            return 1;
        } else if (strcmp(old_key, new_key) < 0) {
            return -1;
        }
    }
    return 0;
}


Country *add_node(Country *node, char *key, char *names[NAMES_NUM]) {
    if (!node) {
        return create_node(key, names);
    }

    if (compare_keys(node->key, key) > 0) { // node->key > key
        if (node->left) {
            return add_node(node->left, key, names);
        } else {
            node->left = create_node(key, names);
            return node->left;
        }
    } else if (compare_keys(node->key, key) < 0) { // node->key < key
        if (node->right) {
            return add_node(node->right, key, names);
        } else {
            node->right = create_node(key, names);
            return node->right;
        }
    }

    return NULL; // klic uz ve strome existuje
}

// only for leaf deleting
void delete_node(Country *node) {
    if (!node) {
        return;
    }
    free(node->key);
    free(node->names);
    free(node);
}

// deletes whole tree from leaves to root
void delete_tree(Country *node) {
    if (node->left) {
        delete_tree(node->left);
    }
    if (node->right) {
        delete_tree(node->right);
    }
    delete_node(node);
}

// counts all nodes of tree/subtree including root
int count_nodes(Country *node) {
    int count = 1;

    if (!node) { // leaf has been found
        return 0;
    }
    count += count_nodes(node->left); // counts number of nodes of subtree
    count += count_nodes(node->right);

    return count;
}

// finds node with key or return null
Country *search_tree(Country *node, char *key) {
    if (!node) {
        return NULL;
    }

    if (compare_keys(node->key, key) > 0) { // node->key > key
        return search_tree(node->left, key);
    }
    if (compare_keys(node->key, key) < 0) { // node->key < key
        return search_tree(node->right, key);
    }

    return node;
}

// counts layers of tree (length of longest branch)
int count_layers(Country *node) {
    int left_depth = 1;
    int right_depth = 1;

    if (!node) {
        return 0;
    }
    left_depth += count_layers(node->left);
    right_depth += count_layers(node->right);

    // after every call we are returning a bigger of both depths
    return (left_depth > right_depth) ? left_depth : right_depth;
}
//////////////////////////////

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
