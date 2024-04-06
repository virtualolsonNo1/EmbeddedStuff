/*--------------------------------------------------------------------
Name: Carl Olson
Date: 7 Mar 24
Course: CSCE 336 Embedded Systems (Spring 2024) 
File: main.cpp
HW/Lab: Homework 4

Purp: Implements code needed to complete lab3 with servo and ultrasonic sensor for demos

Doc:    None

Academic Integrity Statement: I certify that, while others may have 
assisted me in brain storming, debugging and validating this program, 
the program itself is my own work. I understand that submitting code 
which is the work of other individuals is a violation of the honor   
code.  I also understand that if I knowingly give my original work to 
another individual is also a violation of the honor code.
-------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <ctype.h>

uint8_t message[] = {0xef, 0xc3, 0xc2, 0xcb, 0xde, 0xcd, 0xd8, 0xd9, 0xc0, 0xcd, 0xd8, 0xc5, 0xc3,
                    0xc2, 0xdf, 0x8d, 0x8c, 0x8c, 0xf5, 0xc3, 0xd9, 0x8c, 0xc8, 0xc9, 0xcf, 0xde,
                    0xd5, 0xdc, 0xd8, 0xc9, 0xc8, 0x8c, 0xd8, 0xc4, 0xc9, 0x8c, 0xef, 0xff, 0xef,
                    0xe9, 0x9f, 0x9f, 0x9a, 0x8c, 0xc4, 0xc5, 0xc8, 0xc8, 0xc9, 0xc2, 0x8c, 0xc1,
                    0xc9, 0xdf, 0xdf, 0xcd, 0xcB, 0xc9, 0x8c, 0xcd, 0xc2, 0xc8, 0x8c, 0xcd, 0xCf,
                    0xc4, 0xc5, 0xc9, 0xda, 0xc9, 0xc8, 0x8c, 0xde, 0xc9, 0xdd, 0xd9, 0xc5, 0xde,
                    0xc9, 0xc8, 0x8c, 0xca, 0xd9, 0xc2, 0xcf, 0xd8, 0xc5, 0xc3, 0xc2, 0xcd, 0xc0,
                    0xc5, 0xd8, 0xd5, 0x8f};


uint8_t message2[] = {
    0xf8, 0xb7, 0x46, 0x8c, 0xb2, 0x46, 0xdf, 0xac, 0x42, 0xcb, 0xba, 0x03, 0xc7,
0xba, 0x5a, 0x8c, 0xb3, 0x46, 0xc2, 0xb8, 0x57, 0xc4, 0xff, 0x4a, 0xdf, 0xff,
0x12, 0x9a, 0xff, 0x41, 0xc5, 0xab, 0x50, 0x82, 0xff, 0x03, 0xe5, 0xab, 0x03,
0xc3, 0xb1, 0x4f, 0xd5, 0xff, 0x40, 0xc3, 0xb1, 0x57, 0xcd, 0xb6, 0x4d, 0xdf,
0xff, 0x4f, 0xc9, 0xab, 0x57, 0xc9, 0xad, 0x50, 0x80, 0xff, 0x53, 0xc9, 0xad,
0x4a, 0xc3, 0xbb, 0x50, 0x80, 0xff, 0x42, 0xc2, 0xbb, 0x03, 0xdf, 0xaf, 0x42,
0xcf, 0xba, 0x50
};

uint8_t message3[] = {
    0x35, 0xdf, 0x00, 0xca, 0x5d, 0x9e, 0x3d, 0xdb, 0x12, 0xca, 0x5d, 0x9e, 0x32,
0xc8, 0x16, 0xcc, 0x12, 0xd9, 0x16, 0x90, 0x53, 0xf8, 0x01, 0xd7, 0x16, 0xd0,
0x17, 0xd2, 0x0a, 0x90, 0x53, 0xf9, 0x1c, 0xd1, 0x17, 0x90, 0x53, 0xf9, 0x1c,
0xd1, 0x17, 0x9e
};

uint8_t decryptByte(uint8_t encVal, uint8_t key) {
    return encVal ^ key;
}

void decryptMessage(uint8_t* message, uint8_t messageLength, uint8_t keyLength, uint8_t* key, uint8_t* addr) {
    volatile int x = key[0];
    volatile int y = key[1];
    volatile int z = key[2];
    for(int i = 0; i < messageLength; i++) {
        
        addr[i] = decryptByte(message[i], key[i % keyLength]);
    }

    printMessage(addr, messageLength);

}

void printMessage(uint8_t* addr, uint8_t messageLength) {
    // for(int i = 0; i < messageLength; i++) {
    //     Serial.print(addr[i]);
    // }
    // Serial.println();

}


bool containsValidCharacters(unsigned char *message, int messageLength) {
    for (int i = 0; i < messageLength; i++) {
        if (!isalpha(message[i]) && message[i] != '.' && message[i] != ' ')
            return false;
    }
    return true;
}

void decodeMessage(uint8_t *message, uint8_t messageLength, uint16_t key) {
    int i;
    for (i = 0; i < messageLength; i++) {
        message[i] ^= (i % 2 == 0) ? (key >> 8) & 0xFF : key & 0xFF;
    }
}

int main(int argc, char *argv[]) {
    // Your code here
    uint8_t addr[95];
    uint8_t key[] = {0xac};
    decryptMessage(message, 95, 1, key, addr);

    uint8_t addr2[81];
    uint8_t key2[] = {0xac, 0xdf, 0x23};
    decryptMessage(message2, 95, 3, key2, addr);

    FILE *outputFile;
    outputFile = fopen("decoded_messages1.txt", "w");
    if (outputFile == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    uint16_t key3 = 0;

    for (key3 = 0; key3 < 65536; key3++) {
        unsigned char decodedMessage[42];
        for (int i = 0; i < 42; i++) {
            decodedMessage[i] = message3[i];
        }

        decodeMessage(decodedMessage, 42, key3);
        
        if (containsValidCharacters(decodedMessage, 42)) {
            fprintf(outputFile, "Key: %04X - ", key3);
            for (int i = 0; i < 42; i++) {
                fprintf(outputFile, "%c", decodedMessage[i]);
            }
            fprintf(outputFile, "\n");
            return 0;
        }
    }
    return 0;
}