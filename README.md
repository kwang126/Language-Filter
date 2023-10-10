# Assignment 7

This assignment is a language filter that takes in input and detects any badspeak
(strictly forbidden word) and oldspeak (has a replacement newspeak word). If only
badspeak is detected, a disciplinary message is printed. If only oldspeak is detected,
a warning message is printed. If both are detected, a message serving to notify the user
of disciplinary action and warn the user is printed. This filter utilizes a Bloom filter,
hash table, linked list, bit vector, SPECK cipher, and regex parser to operate. The
SPECK cipher and regex parser are provided by Professor Darrell Long.

## Build

        $ make
        
        or

        $ make all

        or

        $ make banhammer

        (These all have the same effect: building the banhammer program)

## Running

        $ ./banhammer -[hsm] -[t <size>] -[f <size>]

        -h Prints a help message explaining program usage.
        -s Enables printing of statistics, replaces regular program output.
        -m Enables move-to-front rule.
        -t Allows user to specify a hash table size. Default is 10000
        -f Allows user to specify a Bloom filter size. Default is 2^20

## Cleaning

        $ make clean
