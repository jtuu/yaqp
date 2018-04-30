#include <stdlib.h>
#include <string.h>

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
            free(cursor->data);
            free(cursor);
            cursor = tmp;
        }
    }
}

node_t* reverse_nodes(node_t *head) {
    node_t *prev = NULL;
    node_t *cur = head;
    node_t *next;

    while (cur != NULL) {
        next = cur->next;
        cur->next = prev;
        prev = cur;
        cur = next;
    }

    head = prev;
    return head;
}

node_t* sort_nodes(node_t *head) {
    node_t *x, *y, *e;

    x = head;
    head = NULL;

    while (x != NULL) {
        e = x;
        x = x->next;

        if (head != NULL) {
            if (e->key > head->key) {
                y = head;
                
                while ((y->next != NULL) && (e->key > y->next->key)) {
                    y = y->next;
                }

                e->next = y->next;
                y->next = e;
            } else {
                e->next = head;
                head = e;
            }
        } else {
            e->next = NULL;
            head = e;
        }
    }

    return head;
}
