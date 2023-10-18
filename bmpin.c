#include <stdio.h>
#include <stdlib.h>
#include "bmpin.h"

unsigned compute_bmp_row_size(BMPfile*);
void free_bmp_file(BMPfile*);
BITMAPFILEHEADER* read_bitmap_file_header(FILE*);
BITMAPINFOHEADER* read_bitmap_info_header(FILE*);
BMPfile* read_bmp_file(const char* const);

inline unsigned compute_bmp_row_size(BMPfile* file) {
    return ((file->info_header->biBitCount)*(file->info_header->biWidth)+31)/32*4;
}

BMPfile* read_bmp_file(const char* const path) {
    FILE* file = fopen(path, "rb");
    if (!file) {
        fprintf(stderr, "File %s not found\n", path);
        exit(1);
    }

    // create IMG object
    BMPfile* img = (BMPfile*)malloc(sizeof(BMPfile));
    img->file_header = read_bitmap_file_header(file);
    img->info_header = read_bitmap_info_header(file);
    img->row_length = compute_bmp_row_size(img);

    // create 2D array
    img->pxarray = (unsigned char**)calloc(img->info_header->biHeight, sizeof(unsigned char*));
    for (unsigned i=0; i<img->info_header->biHeight; i++) {
        img->pxarray[i] = (unsigned char*)calloc(img->row_length, sizeof(unsigned char));
    }

    // Set the file position indicator in front of the pixel array
    fseek(file, img->file_header->bfOffBits, SEEK_SET);
    
    // Read each of a GRB pixel
    for (unsigned i=0; i<img->info_header->biHeight; i++) {
        fread(img->pxarray[i], img->row_length, 1, file);
    }
    fclose(file);
    return img;
}

BITMAPFILEHEADER* read_bitmap_file_header(FILE* file) {
    BITMAPFILEHEADER* res = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
    fread(&res->bfType, sizeof(res->bfType), 1, file);
    fread(&res->bfSize, sizeof(res->bfSize), 1, file);
    fread(&res->bfReserved1, sizeof(res->bfReserved1), 1, file);
    fread(&res->bfReserved2, sizeof(res->bfReserved2), 1, file);
    fread(&res->bfOffBits, sizeof(res->bfOffBits), 1, file);
    return res;
}

BITMAPINFOHEADER* read_bitmap_info_header(FILE* file) {
    BITMAPINFOHEADER* res = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
    fread(&res->biSize, sizeof(res->biSize), 1, file);
    fread(&res->biWidth, sizeof(res->biWidth), 1, file);
    fread(&res->biHeight, sizeof(res->biHeight), 1, file);
    fread(&res->biPlanes, sizeof(res->biPlanes), 1, file);
    fread(&res->biBitCount, sizeof(res->biBitCount), 1, file);
    fread(&res->biCompression, sizeof(res->biCompression), 1, file);
    fread(&res->biSizeImage, sizeof(res->biSizeImage), 1, file);
    fread(&res->biXPelsPerMeter, sizeof(res->biXPelsPerMeter), 1, file);
    fread(&res->biYPelsPerMeter, sizeof(res->biYPelsPerMeter), 1, file);
    fread(&res->biClrUsed, sizeof(res->biClrUsed), 1, file);
    fread(&res->biClrImportant, sizeof(res->biClrImportant), 1, file);
    return res;
}

void free_bmp_file(BMPfile* img) {
    for (unsigned i=0; i<img->info_header->biHeight; i++) {
        free(img->pxarray[i]);
    }
    free(img->pxarray);
    free(img->info_header);
    free(img->file_header);
    free(img);
}
