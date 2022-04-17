#include <stdio.h>
#include <stdlib.h>
#include "bmpout.h"
#include "steganography.h"

bool should_decode_steganography(const BMPfile* const bmp) {
    if (!(bmp->info_header->biCompression == 0 && bmp->info_header->biBitCount == 24)) {
        printf("Steganography not supported for 24-bit\n");
        return false;
    }
    char input;
    // repeat after user input will be valid
    while (true) {
        printf("Decode steganography? [Y/n]\n");
        scanf("%c", &input);
        if (input == 'n' || input == 'N')
            return false;
        if (input == 'y' || input == 'Y')
            return true;
        printf("Invalid option\n");
    }
}

unsigned char decode_oct(const unsigned char arr[]) {
    unsigned char decoded = '\0';
    for (int i=7; i >= 0; --i) {
        // get the least significant bit from arr, then put to the i-th position of the result
        decoded |= (arr[7-i]<<7>>i);
    }
    return decoded;
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

void option_decrypt_steganography(const BMPfile* bmp) {
   unsigned char buf[8] = { 0 };
   fetch_eight_chars(buf, bmp, 0);
   const unsigned short DECRYPT_LEN = decode_oct(buf);
   unsigned char* decrypted_msg = calloc(sizeof(unsigned char), (DECRYPT_LEN+1));
   for (unsigned short i=0; i<DECRYPT_LEN; i++) {
       fetch_eight_chars(buf, bmp, i*8);
       decrypted_msg[i] = decode_oct(buf);
   }
   decrypted_msg[DECRYPT_LEN] = '\0';
   printf("%s\n", decrypted_msg);
   free(decrypted_msg);
}

// TODO: write function
void encrypt_steganography(const BMPfile* bmp, const char msg_to_encrypt[]) {

}

// TODO: write function
void option_encrypt_steganography(const BMPfile* bmp, const char path[], const char msg_to_encrypt[]) {
    

    export_bitmap(bmp, path);
}