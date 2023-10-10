#include "bf.h"
#include "ht.h"
#include "messages.h"
#include "parser.h"

#include <ctype.h>
#include <fcntl.h>
#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>

#define OPTIONS "ht:f:ms"

// Prints help message explaining program usage.
static inline void help(void) {
    printf("SYNOPSIS\n  A word filtering program for the GPRSC.\n  ");
    printf("Filters out and reports bad words parsed from stdin.\n\n");
    printf("USAGE\n  ./banhammer [-hsm] [-t size] [-f size]\n\nOPTIONS\n");
    printf("  %-13sProgram usage and help.\n", "-h");
    printf("  %-13sPrint program statistics.\n", "-s");
    printf("  %-13sEnable move-to-front rule.\n", "-m");
    printf("  %-13sSpecify hash table size (default: 10000).\n", "-t size");
    printf("  %-13sSpecify Bloom filter size (default: 2^20).\n", "-f size");
    return;
}

int main(int argc, char **argv) {
    bool stats = false; // Determine if statistics are printed
    bool mtf = false; // Determine if move-to-front rule is enabled
    uint32_t hash_size = 10000; // Hash table size, default is 10000
    uint32_t bloom_size = 1048576; // Bloom filter size, default is 2^20 (1048576)
    int8_t opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': help(); return 0;

        case 't': hash_size = strtoul(optarg, NULL, 10); break;

        case 'f': bloom_size = strtoul(optarg, NULL, 10); break;

        case 'm': mtf = true; break;

        case 's': stats = true; break;

        default: break;
        }
    }

    BloomFilter *bf = bf_create(bloom_size);
    HashTable *ht = ht_create(hash_size, mtf);

    FILE *bad = fopen("badspeak.txt", "rb");

    if (!bad) {
        printf("Failed to open badspeak.txt.\n");
        bf_delete(&bf);
        ht_delete(&ht);
        return 1;
    }

    FILE *new = fopen("newspeak.txt", "rb");

    if (!new) {
        printf("Failed to open newspeak.txt.\n");
        bf_delete(&bf);
        ht_delete(&ht);
        fclose(bad);
        return 1;
    }

    char word[1028]; // String that is scanned in
    char alt[1028]; // Newspeak string that is scanned in

    // Scanning in badspeak
    while ((fscanf(bad, "%s", word)) != EOF) {
        bf_insert(bf, word);
        ht_insert(ht, word, NULL);
    }

    fclose(bad);

    // Scanning in newspeak
    while ((fscanf(new, "%s %s", word, alt)) != EOF) {
        bf_insert(bf, word);
        ht_insert(ht, word, alt);
    }

    fclose(new);

    LinkedList *badspeak = ll_create(mtf); // List of discovered badspeak
    LinkedList *oldspeak = ll_create(mtf); // List of discovered oldspeak
    Node *found = NULL; // Word found in the hash table
    uint16_t i = 0; // Index in the string when converting to lowercase
    regex_t regex; // Regex of a valid word
    int check = regcomp(&regex, "[A-Za-z0-9_]+(('|-)[A-Za-z0-9_]+)*", REG_EXTENDED);
    char *returned_word; // Word returned from next_word

    // Check to see if regex compiled successfully
    if (check) {
        printf("Regular expression failed to compile.\n");
        ll_delete(&badspeak);
        ll_delete(&oldspeak);
        bf_delete(&bf);
        ht_delete(&ht);
        return 1;
    }

    while ((returned_word = next_word(stdin, &regex)) != NULL) {

        // Converting all strings to lowercase
        while (returned_word[i] != '\0') {
            returned_word[i] = tolower(returned_word[i]);
            i += 1;
        }

        i = 0;

        // If word may have been added to the bloom filter, search hash table
        if (bf_probe(bf, returned_word)) {
            if ((found = ht_lookup(ht, returned_word)) != NULL) {

                // If there is newspeak, it is oldspeak
                if (found->newspeak) {
                    ll_insert(oldspeak, found->oldspeak, found->newspeak);

                    // If there is only oldspeak, it is badspeak
                } else if (found->oldspeak && !(found->newspeak)) {
                    ll_insert(badspeak, found->oldspeak, NULL);
                }
            }
        }
    }

    regfree(&regex);

    if (stats) {
        printf("Seeks: %" PRIu64 "\n", seeks);
        printf("Average seek length: %.6lf\n", ((double) links / seeks));
        printf("Hash table load: %.6lf%%\n", (100 * ((double) ht_count(ht) / hash_size)));
        printf("Bloom filter load: %.6lf%%\n", (100 * ((double) bf_count(bf) / bloom_size)));
    } else {
        if (ll_length(badspeak) > 0 && ll_length(oldspeak) > 0) {
            printf("%s", mixspeak_message);
            ll_print(badspeak);
            ll_print(oldspeak);
        } else if (ll_length(badspeak) > 0 && ll_length(oldspeak) == 0) {
            printf("%s", badspeak_message);
            ll_print(badspeak);
        } else if (ll_length(oldspeak) > 0 && ll_length(badspeak) == 0) {
            printf("%s", goodspeak_message);
            ll_print(oldspeak);
        }
    }

    bf_delete(&bf);
    ht_delete(&ht);
    ll_delete(&badspeak);
    ll_delete(&oldspeak);

    return 0;
}
