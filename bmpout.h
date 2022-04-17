#ifndef BMPOUT_H_INCLUDED
#define BMPOUT_H_INCLUDED
#include "bmp.h"

/**
 * @brief Exports bitmap to the specified path
 * 
 * @param bmp BMPfile object to export
 * @param path A path to the resulting BMP file
 */
void export_bitmap(const BMPfile* bmp, const char path[]);

/**
 * @brief Prints specific file's header
 * 
 * @param f Object to print
 */
void print_bitmap_file_header(struct tagBITMAPFILEHEADER* f);

/**
 * @brief Prints specific file's info
 * 
 * @param f Object to print
 */
void print_bitmap_info_header(struct tagBITMAPINFOHEADER* f);

unsigned char calculate_gray(const struct rgb obj);

void read_histogram(const BMPfile* file);

void option_grayscale(const BMPfile* file, const char* const path);

#endif // BMPOUT_H_INCLUDED
