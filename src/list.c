#include <stdlib.h>
#include <strings.h>

#include "list.h"

static node_t* create_node(int key, size_t data_sz, void *data, node_t* next) {
    node_t *new_node = (node_t *) malloc(sizeof(node_t));
    new_node->key = key;
    new_node->next = next;

    new_node->data = malloc(data_sz);
    memcpy(new_node->data, data, data_sz);

    return new_node;
}

node_t* prepend_node(node_t *head, int key, size_t data_sz, void *data) {
    node_t *new_node = create_node(key, data_sz, data, head);
    head = new_node;
    return head;
}

node_t* append_node(node_t *head, int key, size_t data_sz, void *data) {
    node_t *cursor = head;
    while (cursor->next != NULL) {
        cursor = cursor->next;
    }

    node_t *new_node = create_node(key, data_sz, data, NULL);
    cursor->next = new_node;

    return head;
}

node_t* find_node(node_t *head, int key) {
    node_t *cursor = head;
    while (cursor != NULL) {
        if (cursor->key == key) {
            return cursor;
        }
        cursor = cursor->next;
    }
    return NULL;
}

void traverse_nodes(node_t *head, node_traversal_callback_t cb) {
    node_t *cursor = head;
    while (cursor != NULL) {
        cb(cursor);
        cursor = cursor->next;
    }
}

void dispose_nodes(node_t *head) {
    node_t *cursor, *tmp;
    if (head != NULL) {
        cursor = head->next;
        head->next = NULL;
        while (cursor != NULL) {
            tmp = cursor->next;
            free(cursor);
            cursor = tmp;
        }
    }
}
