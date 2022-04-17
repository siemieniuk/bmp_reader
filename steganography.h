#ifndef STEGANOGRAPHY_H_INCLUDED
#define STEGANOGRAPHY_H_INCLUDED
#include "bmp.h"
#include <stdbool.h>

bool should_decode_steganography(const BMPfile* const bmp);

void option_decode_steganography(const BMPfile* bmp);

void option_encode_steganography(const BMPfile* bmp, const char path[], const char msg_to_encrypt[]);

#endif // STEGANOGRAPHY_H_INCLUDED