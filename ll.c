#include "ll.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t seeks = 0;
uint64_t links = 0;

// LinkedList ADT from Professor Darrell Long via asgn7-v2.pdf
struct LinkedList {
    uint32_t length;
    Node *head;
    Node *tail;
    bool mtf;
};

LinkedList *ll_create(bool mtf) {
    LinkedList *ll = (LinkedList *) malloc(sizeof(LinkedList));

    if (ll) {
        ll->length = 0;
        ll->head = node_create(NULL, NULL);
        ll->tail = node_create(NULL, NULL);
        ll->head->next = ll->tail;
        ll->head->prev = NULL;
        ll->tail->prev = ll->head;
        ll->tail->next = NULL;
        ll->mtf = mtf;
    }

    return ll;
}

void ll_delete(LinkedList **ll) {
    if (*ll) {
        Node *current = (*ll)->head;
        Node *next = current->next;

        // Go through list and delete all Nodes
        while (current) {
            node_delete(&current);
            current = next;

            if (current != (*ll)->tail && current != NULL) {
                next = current->next;
            } else {
                next = NULL;
            }
        }

        free(*ll);
        *ll = NULL;
    }

    return;
}

uint32_t ll_length(LinkedList *ll) {
    return ll->length;
}

Node *ll_lookup(LinkedList *ll, char *oldspeak) {
    Node *result = NULL; // Result of lookup
    Node *current = ll->head->next; // Node we are currently on

    while (current->oldspeak != NULL) {

        // If the current node's oldspeak matches the given oldspeak
        if (strcmp(current->oldspeak, oldspeak) == 0) {
            result = current;

            // Moving current node to the front of the list
            if (ll->mtf) {
                current->prev->next = current->next;
                current->next->prev = current->prev;
                current->prev = ll->head;
                current->next = ll->head->next;
                ll->head->next->prev = current;
                ll->head->next = current;
            }

            break;
        }

        current = current->next;
        links += 1;
    }

    seeks += 1;

    return result;
}

void ll_insert(LinkedList *ll, char *oldspeak, char *newspeak) {

    // Check if oldspeak already exists within the list
    if (ll_lookup(ll, oldspeak)) {
        return;
    }

    // Insert at front of list
    Node *new = node_create(oldspeak, newspeak);
    new->prev = ll->head;
    new->next = ll->head->next;
    ll->head->next->prev = new;
    ll->head->next = new;
    ll->length += 1;

    return;
}

void ll_print(LinkedList *ll) {
    Node *current = ll->head->next;

    while (current->next != NULL) {
        node_print(current);
        current = current->next;
    }

    return;
}
