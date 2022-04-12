#include <stdio.h>
#include "bmpall.h"

void print_bitmap_file_header(struct tagBITMAPFILEHEADER* f) {
    printf("BITMAPFILEHEADER:\n");
    printf("  bfType:      0x%x (BM)\n", f->bfType);
    printf("  bfSize:      %u\n", f->bfSize);
    printf("  bfReserved1: 0x%x\n", f->bfReserved1);
    printf("  bfReserved2: 0x%x\n", f->bfReserved2);
    printf("  bfOffBits:   %u\n", f->bfOffBits);
}

void print_bitmap_info_header(struct tagBITMAPINFOHEADER* f) {
    printf("BITMAPINFOHEADER:\n");
    printf("  biSize:          %d\n", f->biSize);
    printf("  biWidth:         %d\n", f->biWidth);
    printf("  biHeight:        %d\n", f->biHeight);
    printf("  biPlanes:        %d\n", f->biPlanes);
    printf("  biBitCount:      %d\n", f->biBitCount);
    printf("  biCompression:   %d\n", f->biCompression);
    printf("  biSizeImage:     %d\n", f->biSizeImage);
    printf("  biXPelsPerMeter: %d\n", f->biXPelsPerMeter);
    printf("  biYPelsPerMeter: %d\n", f->biYPelsPerMeter);
    printf("  biClrUsed:       %d\n", f->biClrUsed);
    printf("  biClrImportant:  %d\n", f->biClrImportant);
}

inline unsigned get_bmp_row_size(BMPfile* file) {
    return ((file->info_header->biBitCount)*(file->info_header->biWidth)+31)/32*4;
}

void read_histogram(BMPfile* file) {
    if (!(file->info_header->biCompression == 0 && file->info_header->biBitCount == 24)) {
        printf("A histogram calculation is not supported for biCompression != 0 or biBitCount != 24");
        return;
    }
    // TODO: Write a function
}

inline unsigned char calculate_gray(const struct rgb obj) {
    return (obj.r+obj.g+obj.r)/3;
}