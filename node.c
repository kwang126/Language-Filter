#include "node.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *copy_string(char *orig) {
    uint32_t size = strlen(orig) + 1;

    // Allocate enough space for the copy string to hold the original string
    char *copy = (char *) calloc(size, sizeof(char));

    // Copying each char from the original over to the copy
    for (uint32_t i = 0; i < size; i += 1) {
        copy[i] = orig[i];
    }

    return copy;
}

Node *node_create(char *oldspeak, char *newspeak) {
    Node *n = (Node *) malloc(sizeof(Node));

    if (n) {
        if (oldspeak) {
            n->oldspeak = copy_string(oldspeak);
        } else {
            n->oldspeak = NULL;
        }

        if (newspeak) {
            n->newspeak = copy_string(newspeak);
        } else {
            n->newspeak = NULL;
        }

        n->next = NULL;
        n->prev = NULL;
    }

    return n;
}

void node_delete(Node **n) {
    if (*n) {

        // Need to free any space allocated for oldspeak/newspeak strings
        if ((*n)->oldspeak) {
            free((*n)->oldspeak);
            (*n)->oldspeak = NULL;
        }

        if ((*n)->newspeak) {
            free((*n)->newspeak);
            (*n)->newspeak = NULL;
        }

        free(*n);
        *n = NULL;
    }

    return;
}

void node_print(Node *n) {
    if (n) {
        if (n->oldspeak && n->newspeak) {
            printf("%s -> %s\n", n->oldspeak, n->newspeak);
        } else if (n->oldspeak && !(n->newspeak)) {
            printf("%s\n", n->oldspeak);
        } else {
            printf("SENTINEL NODE\n");
        }
    } else {
        printf("NULL\n");
    }

    return;
}
