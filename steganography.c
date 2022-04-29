#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bmpout.h"
#include "steganography.h"

/**
 * @brief Decodes steganography of eight consecutive bytes.
 * @details After each iteration, this function gets least significant bit from i-th element of arr, then puts its value to the decoded character on (7-i)th position.
 * 
 * @param arr_to_decrypt Array of length 8 containing steganography.
 * @return unsigned char Decoded character
 */
unsigned char decode_oct(const unsigned char arr_to_decrypt[]) {
    unsigned char decoded = '\0';
    unsigned char help;
    for (int i=7; i >= 0; --i) {
        help = arr_to_decrypt[7-i];
        help <<= 7;
        help >>= i;
        decoded |= help;
    }
    return decoded;
}

/**
 * @brief Encodes steganography into the BMP object from the specific index.
 * 
 * @param bmp BMP image to modify
 * @param key Character to encrypt
 * @param start Starting byte of pxarray
 */
void encode_oct(const BMPfile* bmp, unsigned char key, const unsigned start) {
    const unsigned ROW_LEN = bmp->row_length;
    for (unsigned i=0; i<8; ++i) {
        if ((key & 1) == 1) {
            bmp->pxarray[(start+i)/ROW_LEN][(start+i)%ROW_LEN] &= 0xFF;
        } else {
            bmp->pxarray[(start+i)/ROW_LEN][(start+i)%ROW_LEN] &= 0xFE;
        }
        key >>= 1;
    }
}

void fetch_eight_chars(unsigned char chars_to_fetch[], const BMPfile* bmp, unsigned start) {
    if (start+7 > bmp->info_header->biSizeImage) {
        fprintf(stderr, "Image is too small to decode steganography\n");
        return;
    }
    const unsigned ROW_LEN = bmp->row_length;
    for (unsigned i=0; i<8; ++i) {
        chars_to_fetch[i] = bmp->pxarray[(start+i)/ROW_LEN][(start+i)%ROW_LEN];
    }
}

/**
 * @brief Checks if BMP image is large enough to encode steganography.
 * 
 * @param bmp BMP file in which steganography will be used
 * @param msg_to_encode Message to encrypt
 * @return True if steganography can be applied without any issues, false otherwise
 */
bool is_bmp_able_to_encode_steganography(const BMPfile* const bmp, const char msg_to_encode[]) {
    return ((strlen(msg_to_encode)+1)*8 > bmp->info_header->biSizeImage) ? false : true;
}

/**
 * @brief Checks if BMP image should be decoded (user wants and if it is possible)
 * 
 * @param bmp BMP file
 * @return True if image should be decoded, false otherwise
 */
bool should_decode_steganography(const BMPfile* const bmp) {
    if (!(bmp->info_header->biCompression == 0 && bmp->info_header->biBitCount == 24)) {
        printf("Steganography is not supported for 24-bit\n");
        return false;
    }
    
    unsigned char buf[8] = { 0 };
    fetch_eight_chars(buf, bmp, 0);
    unsigned char DECRYPT_LEN = decode_oct(buf);
    if (DECRYPT_LEN*8 > bmp->info_header->biSizeImage) {
        printf("Steganography not supported for this file.\n");
        return false;
    }

    char input;
    // repeat until user input is valid
    while (true) {
        printf("Decode steganography? [Y/n] ");
        scanf("%c", &input);
        if (input == 'n' || input == 'N')
            return false;
        if (input == 'y' || input == 'Y')
            return true;
        printf("Invalid option\n");
    }
}

void option_decode_steganography(const BMPfile* bmp) {
    unsigned char buf[8] = { 0 };
    fetch_eight_chars(buf, bmp, 0);
    const unsigned char DECRYPT_LEN = decode_oct(buf);
    unsigned char* decrypted_msg = calloc(sizeof(unsigned char), (DECRYPT_LEN+1));
    for (unsigned short i=0; i<DECRYPT_LEN; i++) {
        fetch_eight_chars(buf, bmp, (i+1)*8);
        decrypted_msg[i] = decode_oct(buf);
    }
    decrypted_msg[DECRYPT_LEN] = '\0';
    printf("\n%s\n", decrypted_msg);
    free(decrypted_msg);
}

void encode_steganography(const BMPfile* bmp, const char msg_to_encode[]) {
    unsigned MSG_LEN = strlen(msg_to_encode);
    // If message is too large, strip to 255 characters
    if (MSG_LEN > 255) {
        MSG_LEN = 255;
    }
    // encode length of message
    encode_oct(bmp, MSG_LEN, 0);
    // encode message
    for (int i=0; i < MSG_LEN; i++) {
        encode_oct(bmp, msg_to_encode[i], (i+1)*8);
    }
}

void option_encode_steganography(const BMPfile* bmp, const char path[], const char msg_to_encode[]) {
    if (is_bmp_able_to_encode_steganography(bmp, msg_to_encode)) {
        encode_steganography(bmp, msg_to_encode);
        export_bitmap(bmp, path);
    } else {
        fprintf(stderr, "Cannot apply steganography. Image is too small.");
    }
}