#include <stdio.h>
#include "bmpout.h"

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

void write_bitmap_file_header(FILE* file, const BMPfile* bmp) {
    BITMAPFILEHEADER* header = bmp->file_header;
    fwrite(&header->bfType, sizeof(header->bfType), 1, file);
    fwrite(&header->bfSize, sizeof(header->bfSize), 1, file);
    fwrite(&header->bfReserved1, sizeof(header->bfReserved1), 1, file);
    fwrite(&header->bfReserved2, sizeof(header->bfReserved2), 1, file);
    fwrite(&header->bfOffBits, sizeof(header->bfOffBits), 1, file);
}

void write_bitmap_info_header(FILE* file, const BMPfile* bmp) {
    BITMAPINFOHEADER* header = bmp->info_header;
    fwrite(&header->biSize, sizeof(header->biSize), 1, file);
    fwrite(&header->biWidth, sizeof(header->biWidth), 1, file);
    fwrite(&header->biHeight, sizeof(header->biHeight), 1, file);
    fwrite(&header->biPlanes, sizeof(header->biPlanes), 1, file);
    fwrite(&header->biBitCount, sizeof(header->biBitCount), 1, file);
    fwrite(&header->biCompression, sizeof(header->biCompression), 1, file);
    fwrite(&header->biSizeImage, sizeof(header->biSizeImage), 1, file);
    fwrite(&header->biXPelsPerMeter, sizeof(header->biXPelsPerMeter), 1, file);
    fwrite(&header->biYPelsPerMeter, sizeof(header->biYPelsPerMeter), 1, file);
    fwrite(&header->biClrUsed, sizeof(header->biClrUsed), 1, file);
    fwrite(&header->biClrImportant, sizeof(header->biClrImportant), 1, file);
}

void write_bitmap_pxarray(FILE* file, const BMPfile* bmp) {
    const unsigned ROW_LENGTH = bmp->row_length;
    for (unsigned row=0; row < bmp->info_header->biHeight; ++row) {
        for (unsigned col=0; col<ROW_LENGTH; col++) {
            fwrite(&bmp->pxarray[row][col], sizeof(unsigned char), 1, file);
        }
    }
}

void write_bitmap_offset(FILE* file, const BMPfile* bmp) {
    const char ZEROVAL = '\0';
    const int offsize = bmp->file_header->bfOffBits - 14 - sizeof(BITMAPINFOHEADER);
    for (int i=0; i<offsize; i++) {
        fwrite(&ZEROVAL, sizeof(char), 1, file);
    }
}

void export_bitmap(const BMPfile* bmp, const char path[]) {
    FILE* file = fopen(path, "wb");
    if (!file) {
        fprintf(stderr, "Cannot create file with a path: \"%s\".", path);
        fprintf(stderr, "Grayscale file was not created\n");
        return;
    }
    write_bitmap_file_header(file, bmp);
    write_bitmap_info_header(file, bmp);
    write_bitmap_offset(file, bmp);
    write_bitmap_pxarray(file, bmp);
    fclose(file);
}

// TODO: fix reading colors; histogram is wrong for pure red image
void get_histogram_row(const BMPfile* file, float arr[], unsigned start_point) {
    for (int i=0; i<16; ++i) {
        arr[i] = 0.0;
    }

    unsigned all_occurences_of_color = 0;
    // group colors into 16 different groups
    for (unsigned row=0; row < file->info_header->biHeight; ++row) {
        for (unsigned col=start_point; col < file->row_length; ++col) {
            arr[(file->pxarray[row][col])/16]++;
            all_occurences_of_color++;
        }
    }
    // change to ratio
    for (unsigned i=0; i<16; ++i) {
        arr[i] /= all_occurences_of_color;
    }

}

void print_histogram(const float arr[], const char* color_descriptor) {
    printf("%s:\n", color_descriptor);
    for (unsigned i=0; i<16; i++) {
        printf("  %d-%d: %.2f%%\n", i*16, i*16+15, arr[i]*100);
    }
}

void read_histogram(const BMPfile* file) {
    if (!(file->info_header->biCompression == 0 && file->info_header->biBitCount == 24)) {
        printf("A histogram is not supported for biCompression != 0 or biBitCount != 24\n");
        return;
    }
    float color_share_arr[16];
    get_histogram_row(file, color_share_arr, 0);
    print_histogram(color_share_arr, "Blue");
    get_histogram_row(file, color_share_arr, 1);
    print_histogram(color_share_arr, "Green");
    get_histogram_row(file, color_share_arr, 2);
    print_histogram(color_share_arr, "Red");
}

void convert_to_grayscale(const BMPfile* bmp) {
    const unsigned ROW_LENGTH = bmp->row_length;
    for (unsigned row=0; row < bmp->info_header->biHeight; ++row) {
        for (unsigned col=0; col < ROW_LENGTH-2; col+=3) {
            struct rgb obj = {
                bmp->pxarray[row][col+2], // Red
                bmp->pxarray[row][col+1], // Green
                bmp->pxarray[row][col]    // Blue
            };
            unsigned char gray = calculate_gray(obj);
            bmp->pxarray[row][col+2] = gray;
            bmp->pxarray[row][col+1] = gray;
            bmp->pxarray[row][col] = gray;
        }
    }
}

void option_grayscale(const BMPfile* bmp, const char* const path) {
    if (!(bmp->info_header->biCompression == 0 && bmp->info_header->biBitCount == 24)) {
        printf("Grayscale is not supported for biCompression != 0 or biBitCount != 24\n");
        return;
    }
    convert_to_grayscale(bmp);
    export_bitmap(bmp, path);
}

inline unsigned char calculate_gray(const struct rgb obj) {
    return (obj.r+obj.g+obj.r)/3;
}