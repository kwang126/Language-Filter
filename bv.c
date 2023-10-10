#include "bv.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Finds the location of a byte given a bit in a bit vector.
#define BYTE(i) (uint32_t)(i / 8)

// Finds the location of a specified bit in a bit vector.
#define BIT(i) (uint32_t)(i % 8)

// BitVector ADT from Professor Darrell Long via asgn5.pdf
struct BitVector {
    uint32_t length;
    uint8_t *vector;
};

BitVector *bv_create(uint32_t length) {
    if (length < 1) {
        return NULL;
    }

    BitVector *bv = (BitVector *) malloc(sizeof(BitVector));

    if (bv) {
        bv->length = length;
        uint32_t bytes; // Amount of bytes the vector is made up of.

        // Do this so we don't accidentally allocate 1 extra unnecessary byte if the
        // amount of bits fits perfectly.
        if ((length % 8) == 0) {
            bytes = length / 8;
            bv->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
        } else {
            bytes = (length / 8) + 1;
            bv->vector = (uint8_t *) calloc(bytes, sizeof(uint8_t));
        }

        if (!bv->vector) {
            free(bv);
            bv = NULL;
        } else {

            // Initialize all digits to 0
            for (uint32_t i = 0; i < bytes; i += 1) {
                bv->vector[i] = 0;
            }
        }
    }

    return bv;
}

void bv_delete(BitVector **bv) {
    if (*bv && (*bv)->vector) {
        free((*bv)->vector);
        free(*bv);
        *bv = NULL;
    }

    return;
}

uint32_t bv_length(BitVector *bv) {
    return bv->length;
}

void bv_set_bit(BitVector *bv, uint32_t i) {

    // 128 (decimal) in binary is 10000000
    // Start with a 1 at the MSB, move right to the index of the bit we want to set.
    uint8_t set = 128 >> BIT(i);
    bv->vector[BYTE(i)] |= set;
    return;
}

void bv_clr_bit(BitVector *bv, uint32_t i) {

    // Do the same thing as bv_set_bit but then NOT it so that all bits but the one we want
    // to clear are 1s.
    uint8_t clr = ~(128 >> BIT(i));
    bv->vector[BYTE(i)] &= clr;
    return;
}

uint8_t bv_get_bit(BitVector *bv, uint32_t i) {

    // Shift 1 right to desired bit's index. Since all indices but the desired one are 0s, we
    // can simply do a bitwise AND, which masks all bits but the desired one. Then shift it
    // right to the LSB.
    uint8_t bit = 128 >> BIT(i);
    bit = bv->vector[BYTE(i)] & bit;
    bit = bit >> (7 - BIT(i));
    return bit;
}

void bv_print(BitVector *bv) {
    for (uint32_t i = 0; i < bv->length; i += 1) {
        uint8_t bit = bv->vector[BYTE(i)] << BIT(i);
        bit = bit >> 7;
        printf("%u ", bit);
    }

    printf("\n");
    return;
}
