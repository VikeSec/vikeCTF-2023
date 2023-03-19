#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/random.h>

#include "monocypher.h"

#define ARGON_SIZE 32
#define BLAKE_SIZE 64
#define KB 1024

#define WORK_AREA_SIZE (KB * 100000)
#define NB_BLOCKS_MAIN 100000
#define NB_ITERATIONS_MAIN 3
#define NONCE_MAIN \
    (uint8_t*)"\x39\x40\xcb\x83\x5a\x3c\xf8\x47\xad\x9c\xda\xc3\n\x07\xe5\x62"

#define NONCE_PRINT \
    (uint8_t*)"\xa0m[\xeb<\xd5\x1a\x8a\x1b\x8f\x90\xaa\x82\x9e.\xf6\x9a\x1b_"  \
              "\xf3\x14\xed\x04&"

/*
// this password hashes to EXPECTED_HASH
#define PASSWORD                                                               \
    (uint8_t*)"you will literally never guess this with brute force"
*/

// c092dfd344921623fc068ead9cceeba182f9483fdcfe418c65397a70a6142e90
#define EXPECTED_HASH \
    (uint8_t*)"\xc0\x92\xdf\xd3\x44\x92\x16\x23\xfc\x06\x8e\xad\x9c\xce\xeb"   \
              "\xa1\x82\xf9\x48\x3f\xdc\xfe\x41\x8c\x65\x39\x7a\x70\xa6\x14"   \
              "\x2e\x90"

#define MAX_PASSWORD_LEN 256

uint8_t flag_ciphertext[] =
    "\xd3\x67\xa7\x97\xa3\x11\xa4\x40\xfc\x46\xcf\xcb\x50\xed\x99\x14\xf4\x7c"
    "\x81\xbb\xd4\x05\x38\xc5\x82\x55\x05\x3e\xb0\x90\x00\x90\xf4\x5e\xfb\x42"
    "\x78\x82\xb2\x40\xff\x70\xed";
uint8_t flag_mac[] =
    "\x2d\x44\xdb\xbe\x13\x4b\x4f\x57\x79\xe4\xa1\xcb\x8c\x4f\x0a\xb1";

// uint8_t flag[] = "vikeCTF{p4tC#_m3_l1k3_1_0F_uR_fR3Nc#_g1r!s}";

uint8_t kdf_last_hash[BLAKE_SIZE];
uint8_t kdf_counter = 1;
void kdf(uint8_t hash[BLAKE_SIZE]) {
    // setup message as last_hash <> counter
    uint8_t message[BLAKE_SIZE + 8] = {0};
    memcpy(message, kdf_last_hash, sizeof kdf_last_hash);
    message[BLAKE_SIZE] = kdf_counter;

    // hash
    crypto_blake2b(hash, message, sizeof message);

    // update state for next call
    memcpy(kdf_last_hash, hash, sizeof kdf_last_hash);
    kdf_counter++;
}

void init_kdf(uint8_t hash[BLAKE_SIZE]) {
    memcpy(kdf_last_hash, hash, sizeof kdf_last_hash);
    kdf_counter = 1;
}

void print_flag(void) {
    // make a blake2b hash to seed the KDF
    uint8_t init_hash[BLAKE_SIZE] = {0};
    crypto_blake2b(init_hash, EXPECTED_HASH, ARGON_SIZE);
    init_kdf(init_hash);

    // expand the KDF over 4 KB
    uint8_t expanded[4 * KB] = {5};
    for (int i = 0; i < (4 * KB) / BLAKE_SIZE; i++) {
        kdf(expanded + i * BLAKE_SIZE);
    }

    // run a rolling XOR over the expanded output
    uint8_t random_number = 14;
    for (int i = 0; i < 4 * KB; i++) {
        expanded[i] = expanded[i] ^ random_number;
        random_number = random_number ^ i;
    }

    uint8_t key[ARGON_SIZE];
    uint8_t* work_area = malloc(NB_BLOCKS_MAIN * KB);
    crypto_argon2i(key, ARGON_SIZE, work_area, NB_BLOCKS_MAIN,
                   NB_ITERATIONS_MAIN, expanded, sizeof expanded, NONCE_MAIN,
                   strlen((char*)NONCE_MAIN));
    free(work_area);

    // // encrypt the flag with the key and hardcode the AEAD in
    // uint8_t ciphertext[sizeof flag - 1];
    // uint8_t mac[16] = {0};
    // crypto_lock(mac, ciphertext, key, NONCE_PRINT, flag,
    // strlen((char*)flag));
    // // fwrite(ciphertext, 1, sizeof ciphertext, stdout);
    // fwrite(mac, 1, sizeof mac, stdout);

    // decrypt the hardcoded AEAD with the key
    uint8_t plaintext[sizeof flag_ciphertext] = {0};
    if (crypto_unlock(plaintext, key, NONCE_PRINT, flag_mac, flag_ciphertext,
                      sizeof flag_ciphertext - 1) == -1) {
        fprintf(stderr, "error decrypting\n");
        exit(1);
    }
    fwrite(plaintext, 1, sizeof flag_ciphertext, stdout);
}

int main(void) {
    fprintf(stderr,
            "          __     __\n"
            "         / < ___ > \\\n"
            "         \'-._____.-\'\n"
            "          ,| ^_^ |,\n"
            "           ((())))\n"
            "             | |\n"
            "        ,############\\\n"
            "       /  #########,  \\\n"
            "      /_<\'#########\'./_\\\n"
            "     \'_7_ ######### _o_7\n"
            "      (  \\[o-o-o-o]/  )\n"
            "       \\|l#########l|/                 __,\'\n"
            "          ####_####                 ,-\'  ,\'\n"
            "         /    |    \\             .-\'  ;  |_\n"
            "~~~~~~~~~|    |    |~~~~~~~~~~~~/_,-./  -._\n"
            "         |_  _|_  _|       ~~ ~   \'--.____.\n"
            "  ~ ~    |\\\\//|\\\\//|             ~ ~~  ~\n"
            "         \\//\\\\|//\\\\/   ~ ~~~~\n"
            "       ___\\\\// \\\\//___\n"
            "      (((___X\\ /X___)))\n");
    fprintf(stderr,
            "\n"
            "Viking: Greetings, traveler! I am Gunnar the Mighty, defender of "
            "this sacred flag. Before I grant you access to this hallowed "
            "possession, I must verify that you are indeed worthy. Tell me, "
            "what is the secret passphrase?\n"
            "\n");
    fprintf(stderr, "You: ");

    uint8_t password[MAX_PASSWORD_LEN + 1];
    fgets((char*)password, MAX_PASSWORD_LEN + 1, stdin);

    size_t password_len = strlen((char*)password);
    if (password[password_len - 1] == '\n') {
        password[password_len - 1] = '\0';
        password_len -= 1;
    }

    uint8_t hash[ARGON_SIZE];
    uint8_t* work_area = malloc(NB_BLOCKS_MAIN * KB);

    crypto_argon2i(hash, ARGON_SIZE, work_area, NB_BLOCKS_MAIN,
                   NB_ITERATIONS_MAIN, password, password_len, NONCE_MAIN,
                   strlen((char*)NONCE_MAIN));
    free(work_area);

    if (memcmp(hash, EXPECTED_HASH, ARGON_SIZE) == 0) {
        fprintf(stderr,
                "\n"
                "You are worthy! Here is the flag, the pride of our people:\n"
                "\n");
        print_flag();
    } else {
        fprintf(stderr,
                "\n"
                "You are unworthy. Here is your punishment:\n");
        fprintf(stderr,
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠿⠿⠿⠛⠿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡉⠁⠈⠁⠀⠀⠀⠀⠀⠀⠈⠹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⣀⣀⡀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣿⣾⣾⣷⣾⣿⣿⣿⣿⣿⡆⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡀⣿⡿⠿⠿⣿⣿⣿⣿⣿⣿⡇⠀⣼⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣷⣿⡆⠀⠀⢹⣿⣷⣚⣿⣿⣿⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣯⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣽⡂⣨⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠉⠉⣻⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣇⡀⢳⣿⣿⣏⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⠛⢹⣿⠀⠁⠉⠹⣿⣿⡗⢻⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⠿⠛⠉⠁⠀⠀⠀⢷⣤⡀⠀⠀⢻⣿⡇⡵⠿⣷⠀⠀⠉⠉⠛⠻⠿⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⣿⣿⡿⠟⠋⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣷⣿⣶⣾⣿⣿⣻⣿⣿⠀⠀⠀⠀⠀⠀⠀⠈⠙⠻⣿⣿⣿\n"
                "⣿⣿⣿⣿⣿⠋⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿\n"
                "⣿⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⢻⣍⣛⡛⠛⢻⣿⣿⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⣿⣿\n"
                "⣿⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⠶⠿⠯⣭⣭⣟⣏⡁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿\n"
                "⣿⣿⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣛⣛⣶⠶⠶⡇⠬⢍⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿\n"
                "⣿⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣭⣭⣭⣟⣛⡃⠒⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿\n"
                "⣿⣿⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠰⠶⠦⣤⣭⡇⣉⠙⠓⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣿⣿\n"
                "⣿⣿⣿⠃⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢘⣛⣓⡲⠶⠆⠤⠬⠍⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿\n"
                "⣿⣿⡟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢭⣍⣉⣉⢀⠂⠀⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⢸⣿⣿\n"
                "⣿⣿⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠒⠲⠦⠤⠰⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⣰⣿⣿\n"
                "⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣙⣛⠓⠒⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⣿⣿\n"
                "⣿⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠦⠠⠀⠈⠈⠉⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢹⣿⣿\n"
                "⣿⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣤⣦⣼⣿⣿\n"
                "⣇⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠐⠀⠀⠀⠀⠀⠀⠀⠀⠀⣽⣿⣿⣿⣿⣿\n"
                "⣿⣧⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⢀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣿⣿⣿⣿⣿⣿\n"
                "⣿⣿⣷⡀⠀⠀⠀⠀⠀⠀⠀⠀⣀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣤⣤⣤⣾⢸⣠⡀⠀⠀⠀⠀⠀⠀⠀⠘⢿⣿⣿⣿⣿\n"
                "\n"
                "https://www.youtube.com/watch?v=dQw4w9WgXcQ\n");
    }

    return 0;
}
