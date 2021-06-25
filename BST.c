#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "BST.h"


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


void delete_node(Country *node) {
    if (!node) {
        return;
    }
    free(node->key);
    for (int i = 0; i < NAMES_NUM; ++i) {
        free(node->names[i]);
    }
    free(node);
}


void delete_tree(Country *node) {
    if (node->left) {
        delete_tree(node->left);
    }
    if (node->right) {
        delete_tree(node->right);
    }
    delete_node(node);
}


int count_nodes(Country *node) {
    int count = 1;

    if (!node) { // leaf has been found
        return 0;
    }
    count += count_nodes(node->left); // counts number of nodes of subtree
    count += count_nodes(node->right);

    return count;
}


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
