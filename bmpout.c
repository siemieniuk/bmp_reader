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

// TODO: fix reading colors; histogram is wrong for pure red image
void get_histogram_row(const BMPfile* file, float arr[], unsigned start_point) {
    for (int i=0; i<16; ++i) {
        arr[i] = 0.0;
    }

    unsigned all_occurences_of_color = 0;
    // group colors into 16 different groups
    for (unsigned row=0; row < file->info_header->biHeight; ++row) {
        for (unsigned col=start_point; col < file->row_length; ++col) {
            arr[(file->content[row][col])/16]++;
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
        printf("A histogram calculation is not supported for biCompression != 0 or biBitCount != 24");
        return;
    }
    // TODO: Write a function
    float color_share_arr[16];
    get_histogram_row(file, color_share_arr, 0);
    print_histogram(color_share_arr, "Blue");
    get_histogram_row(file, color_share_arr, 1);
    print_histogram(color_share_arr, "Green");
    get_histogram_row(file, color_share_arr, 2);
    print_histogram(color_share_arr, "Red");
    
}

inline unsigned char calculate_gray(const struct rgb obj) {
    return (obj.r+obj.g+obj.r)/3;
}