CC = clang
CFLAGS = -Wall -Wextra -Werror -Wpedantic

all: banhammer

banhammer: banhammer.o bv.o ht.o ll.o bf.o node.o speck.o parser.o
	$(CC) -o banhammer banhammer.o bv.o ht.o ll.o bf.o node.o speck.o parser.o

banhammer.o: banhammer.c
	$(CC) $(CFLAGS) -c banhammer.c

bv.o: bv.c
	$(CC) $(CFLAGS) -c bv.c

ht.o: ht.c ll.o speck.o
	$(CC) $(CFLAGS) -c ht.c

ll.o: ll.c node.o
	$(CC) $(CFLAGS) -c ll.c

bf.o: bf.c bv.o speck.o
	$(CC) $(CFLAGS) -c bf.c

node.o: node.c
	$(CC) $(CFLAGS) -c node.c

speck.o: speck.c
	$(CC) $(CFLAGS) -c speck.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

format:
	clang-format -i -style=file *.[ch]

clean:
	rm -f banhammer banhammer.o bv.o ht.o ll.o bf.o node.o speck.o parser.o

scan-build: clean
	scan-build --use-cc=clang make
