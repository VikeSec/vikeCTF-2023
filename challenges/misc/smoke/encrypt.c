#include <stdio.h>
#include <string.h>

#define MAX_FILENAME_LENGTH 256
#define BUFFER_SIZE 1024

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s input_file password\n", argv[0]);
        return 1;
    }

    // Parse command-line arguments
    char *input_filename = argv[1];
    char *password = argv[2];

    // Open input file
    FILE *input_file = fopen(input_filename, "rb");
    if (input_file == NULL) {
        printf("Error: cannot open input file %s\n", input_filename);
        return 1;
    }

    // Compute length of password
    size_t password_length = strlen(password);

    // Open output file with ".enc" extension
    char output_filename[MAX_FILENAME_LENGTH];
    strncpy(output_filename, input_filename, MAX_FILENAME_LENGTH);
    strncat(output_filename, ".enc", MAX_FILENAME_LENGTH - strlen(output_filename) - 1);
    FILE *output_file = fopen(output_filename, "wb");
    if (output_file == NULL) {
        printf("Error: cannot create output file %s\n", output_filename);
        fclose(input_file);
        return 1;
    }

    // Read input file in chunks and XOR with password
    unsigned char buffer[BUFFER_SIZE];
    size_t bytes_read;
    size_t i;
    while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, input_file)) > 0) {
        for (i = 0; i < bytes_read; i++) {
            buffer[i] ^= password[i % password_length];
            buffer[i] += i;
        }
        fwrite(buffer, 1, bytes_read, output_file);
    }

    // Close files
    fclose(input_file);
    fclose(output_file);

    printf("Encryption successful: %s -> %s\n", input_filename, output_filename);

    return 0;
}
