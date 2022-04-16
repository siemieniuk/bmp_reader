#include <stdio.h>
#include <stdlib.h>
#include "bmpall.h"

BITMAPFILEHEADER* read_bitmap_file_header(FILE*);
BITMAPINFOHEADER* read_bitmap_info_header(FILE*);
BMPfile* read_bmp_file(const char* const path);
void free_bmp_file(BMPfile*);

BMPfile* read_bmp_file(const char* const path) {
    FILE* f = fopen(path, "r");
    if (!f) {
        fprintf(stderr, "File &s not found!", path);
        exit(1);
    }
    BMPfile* img = (BMPfile*)malloc(sizeof(BMPfile));
    img->file_header = read_bitmap_file_header(f);
    img->info_header = read_bitmap_info_header(f);
    fclose(f);
    return img;
}

BITMAPFILEHEADER* read_bitmap_file_header(FILE* f) {
    BITMAPFILEHEADER* res = (BITMAPFILEHEADER*)malloc(sizeof(BITMAPFILEHEADER));
    fread(&res->bfType, sizeof(res->bfType), 1, f);
    fread(&res->bfSize, sizeof(res->bfSize), 1, f);
    fread(&res->bfReserved1, sizeof(res->bfReserved1), 1, f);
    fread(&res->bfReserved2, sizeof(res->bfReserved2), 1, f);
    fread(&res->bfOffBits, sizeof(res->bfOffBits), 1, f);
    return res;
}

BITMAPINFOHEADER* read_bitmap_info_header(FILE* f) {
    BITMAPINFOHEADER* res = (BITMAPINFOHEADER*)malloc(sizeof(BITMAPINFOHEADER));
    fread(&res->biSize, sizeof(res->biSize), 1, f);
    fread(&res->biWidth, sizeof(res->biWidth), 1, f);
    fread(&res->biHeight, sizeof(res->biHeight), 1, f);
    fread(&res->biPlanes, sizeof(res->biPlanes), 1, f);
    fread(&res->biBitCount, sizeof(res->biBitCount), 1, f);
    fread(&res->biCompression, sizeof(res->biCompression), 1, f);
    fread(&res->biSizeImage, sizeof(res->biSizeImage), 1, f);
    fread(&res->biXPelsPerMeter, sizeof(res->biXPelsPerMeter), 1, f);
    fread(&res->biYPelsPerMeter, sizeof(res->biYPelsPerMeter), 1, f);
    fread(&res->biClrUsed, sizeof(res->biClrUsed), 1, f);
    fread(&res->biClrImportant, sizeof(res->biClrImportant), 1, f);
    return res;
}

void free_bmp_file(BMPfile* file) {
    free(file->file_header);
    free(file->info_header);
    free(file);
}
