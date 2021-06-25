#ifndef ULOHA_8_BST_H
#define ULOHA_8_BST_H

#define NAMES_NUM 3
#define KEYS_NUM 3

struct Ctr {
    char *key;
    char *names[NAMES_NUM];
    struct Ctr *left;
    struct Ctr *right;
};
typedef struct Ctr Country;

/* create node with given arguments */
Country *create_node(char *key, char *names[NAMES_NUM]);

/* help function for sorting the keys based on the data types */
int compare_keys(char *old_key, char *new_key);

/* add node to BST based on chosen key */
Country *add_node(Country *node, char *key, char *names[NAMES_NUM]);

/* only for leaf deleting */
void delete_node(Country *node);

/* deletes whole tree from leaves to root */
void delete_tree(Country *node);

/* counts all nodes of tree/subtree including root */
int count_nodes(Country *node);

/* finds node with key or return null */
Country *search_tree(Country *node, char *key);

/* counts layers of tree (length of longest branch) */
int count_layers(Country *node);

#endif
