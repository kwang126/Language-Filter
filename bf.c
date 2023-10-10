#include "bf.h"

#include "speck.h"

#include <stdio.h>
#include <stdlib.h>

// BloomFilter ADT from Professor Darrell Long via asgn7-v2.pdf
struct BloomFilter {
    uint64_t primary[2];
    uint64_t secondary[2];
    uint64_t tertiary[2];
    BitVector *filter;
};

// BloomFilter constructor from Professor Darrell Long via asgn7-v2.pdf
BloomFilter *bf_create(uint32_t size) {
    BloomFilter *bf = (BloomFilter *) malloc(sizeof(BloomFilter));

    if (bf) {

        // Grimm's Fairy Tales
        bf->primary[0] = 0x5adf08ae86d36f21;
        bf->primary[1] = 0xa267bbd3116f3957;

        // The Adventures of Sherlock Holmes
        bf->secondary[0] = 0x419d292ea2ffd49e;
        bf->secondary[1] = 0x09601433057d5786;

        // The Strange Case of Dr. Jekyll and Mr. Hyde
        bf->tertiary[0] = 0x50d8bb08de3818df;
        bf->tertiary[1] = 0x4deaae187c16ae1d;
        bf->filter = bv_create(size);

        if (!bf->filter) {
            free(bf);
            bf = NULL;
        }
    }

    return bf;
}

void bf_delete(BloomFilter **bf) {
    if (*bf) {
        bv_delete(&((*bf)->filter));
        free(*bf);
        *bf = NULL;
    }

    return;
}

uint32_t bf_size(BloomFilter *bf) {
    return bv_length(bf->filter);
}

void bf_insert(BloomFilter *bf, char *oldspeak) {
    bv_set_bit(bf->filter, (hash(bf->primary, oldspeak) % bf_size(bf)));
    bv_set_bit(bf->filter, (hash(bf->secondary, oldspeak) % bf_size(bf)));
    bv_set_bit(bf->filter, (hash(bf->tertiary, oldspeak) % bf_size(bf)));

    return;
}

bool bf_probe(BloomFilter *bf, char *oldspeak) {

    // Doing it this way to stay within line length requirement
    uint8_t bit0 = bv_get_bit(bf->filter, (hash(bf->primary, oldspeak) % bf_size(bf)));
    uint8_t bit1 = bv_get_bit(bf->filter, (hash(bf->secondary, oldspeak) % bf_size(bf)));
    uint8_t bit2 = bv_get_bit(bf->filter, (hash(bf->tertiary, oldspeak) % bf_size(bf)));

    if (bit0 || bit1 || bit2) {
        return true;
    }

    return false;
}

uint32_t bf_count(BloomFilter *bf) {
    uint32_t count = 0;

    // Counting up the bits. Since bits are always either 1 or 0, we can count the amount of set
    // bits by simply adding up all of the bits.
    for (uint32_t i = 0; i < bv_length(bf->filter); i += 1) {
        count += bv_get_bit(bf->filter, i);
    }

    return count;
}

void bf_print(BloomFilter *bf) {
    bv_print(bf->filter);

    return;
}
