#ifndef BMPOUT_H_INCLUDED
#define BMPOUT_H_INCLUDED
#include "bmp.h"
/**
 * Prints specific file's header
 * @param f Object to print
 */
void print_bitmap_file_header(struct tagBITMAPFILEHEADER* f);

/**
 * Prints specific file's info
 * @param f Object to print
 */
void print_bitmap_info_header(struct tagBITMAPINFOHEADER* f);

unsigned char calculate_gray(const struct rgb obj);

void read_histogram(BMPfile* file);


#endif // BMPOUT_H_INCLUDED
