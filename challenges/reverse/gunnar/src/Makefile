CC=gcc
CFLAGS= -pedantic -Wall -Wextra -march=native -O0
# CFLAGS= -pedantic -Wall -Wextra -march=native -fsanitize=address -g

.PHONY: all clean

all:
	$(CC) $(CFLAGS) -o challenge challenge.c monocypher.o
	strip -w -K "*" -K '!crypto_*' -K '!blake2b_*' -K '!chacha20*' -K '!*kdf*' -K "!flag_ciphertext" -K "!flag_*" -K "!*.c" challenge
clean:
	rm challenge