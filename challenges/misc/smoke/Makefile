
all: encrypt decrypt

encrypt: encrypt.c
	gcc -o encrypt encrypt.c

decrypt: decrypt.c
	gcc -O3 -s -o decrypt decrypt.c

clean:
	rm -f encrypt decrypt flag.*
