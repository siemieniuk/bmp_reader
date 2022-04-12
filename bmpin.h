#ifndef BMPIN_H_INCLUDED
#define BMPIN_H_INCLUDED
#include "bmp.h"
#include <stdio.h>

/**
 * Creates BMP file object
 * @param path Path to the specific BMP file
 * @return A pointer to BMP file
 */
BMPfile* read_bmp_file(const char* const path) ;

/**
 * Removes BMP file from memory
 * @param file A file to deallocate
 */
void free_bmp_file(BMPfile* file);

#endif // BMPIN_H_INCLUDED