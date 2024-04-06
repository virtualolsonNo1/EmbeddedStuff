#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>

bool containsValidCharacters(unsigned char *message, int messageLength) {
    for (int i = 0; i < messageLength; i++) {
        if (!isalpha(message[i]) && message[i] != '.' && message[i] != ' ')
            return false;
    }
    return true;
}

void decodeMessage(unsigned char *message, int messageLength, unsigned short key) {
    int i;
    for (i = 0; i < messageLength; i++) {
        message[i] ^= (i % 2 == 0) ? (key >> 8) & 0xFF : key & 0xFF;
    }
}

int main() {
    unsigned char message[] = { 0x35, 0xdf, 0x00, 0xca, 0x5d, 0x9e, 0x3d, 0xdb, 0x12, 0xca, 0x5d, 0x9e, 0x32, 0xc8, 0x16, 0xcc, 0x12, 0xd9, 0x16, 0x90, 0x53, 0xf8, 0x01, 0xd7, 0x16, 0xd0, 0x17, 0xd2, 0x0a, 0x90, 0x53, 0xf9, 0x1c, 0xd1, 0x17, 0x90, 0x53, 0xf9, 0x1c, 0xd1, 0x17, 0x9e };
    int messageLength = 42;
    unsigned short key;

    printf("Decoded Messages with Valid Characters:\n");

    FILE *outputFile;
    outputFile = fopen("decoded_messages.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    for (key = 0; key < 65536; key++) {
        unsigned char decodedMessage[42];
        for (int i = 0; i < messageLength; i++) {
            decodedMessage[i] = message[i];
        }

        decodeMessage(decodedMessage, messageLength, key);

        if (containsValidCharacters(decodedMessage, messageLength)) {
            fprintf(outputFile, "Key: %04X - ", key);
            for (int i = 0; i < messageLength; i++) {
                fprintf(outputFile, "%c", decodedMessage[i]);
            }
            fprintf(outputFile, "\n");
            return 0;
        }
    }

    return 0;
}
