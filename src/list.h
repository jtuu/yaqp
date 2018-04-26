#ifndef YAQP_LIST_H
#define YAQP_LIST_H

#include <stdlib.h>

typedef struct node {
    int key;
    void *data;
    struct node *next;
} node_t;

node_t* prepend_node(node_t *head, int key, size_t data_sz, void *data);
node_t* append_node(node_t *head, int key, size_t data_sz, void *data);
node_t* find_node(node_t *head, int key);

typedef void (*node_traversal_callback_t)(node_t* data);
void traverse_nodes(node_t *head, node_traversal_callback_t cb);

void dispose_nodes(node_t *head);

node_t* reverse_nodes(node_t *head);

#endif
