#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define KEY 93
#define FLAG_LEN 27

// vikeCTF{n0_57R1n95_F0r_Y0u}
const char* ciphertext =
    "\x2b\x34\x36\x38\x1e\x09\x1b\x26\x33\x6d\x02\x68\x6a\x0f\x6c\x33\x64\x68"
    "\x02\x1b\x6d\x2f\x02\x04\x6d\x28\x20";

// simply XOR encryption
void encrypt(char* input) {
  for (size_t i = 0; i < FLAG_LEN; i++) {
    input[i] ^= KEY;
  }
}

int main(void) {
  printf(
      "     _.-._\n"
      "   .\' | | `.\n"
      "  /   | |   \\\n"
      " |    | |    |\n"
      " |____|_|____|\n"
      " |____(_)____|\n"
      " /|(o)| |(o)|\\\n"
      "//|   | |   |\\\\\n"
      "\'/|  (|_|)  |\\`\n"
      " //.///|\\\\\\.\\\\\n"
      " /////---\\\\\\\\\\\n"
      " ////|||||\\\\\\\\\n"
      " \'//|||||||\\\\`\n"
      "   \'|||||||`\n"
      "\n"
      "I am the flag oracle.\n"
      "Enter a flag and I will tell you if it is correct.\n"
      "Flag: ");

  char input[FLAG_LEN+1];
  fgets(input, FLAG_LEN+1, stdin);

  encrypt(input);

  if (memcmp(ciphertext, input, FLAG_LEN) == 0) {
    fprintf(stderr, "Success, your input matches the flag :)\n");
    return 0;
  }
  fprintf(stderr, "Failure, your input does not match the flag :(\n");
  return 1;
}