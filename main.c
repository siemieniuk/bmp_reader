#include <stdio.h>
#include <stdlib.h>
#include "bmpall.h"
#include "steganography.h"

void option_invalid_program_usage();

int main(int argc, char* argv[]) {
    BMPfile* bmp;
    // Script accepts only argc from [2,4].
    if (2 <= argc && argc <= 4) {
        bmp = read_bmp_file(argv[1]);
    }
    switch (argc) {
        case 2:
            print_bitmap_file_header(bmp->file_header);
            print_bitmap_info_header(bmp->info_header);
            read_histogram(bmp);
            if (should_decode_steganography(bmp)) {
                option_decode_steganography(bmp);
            }
        break;
        case 3:
            option_grayscale(bmp, argv[2]);
            break;
        case 4:
            option_encode_steganography(bmp, argv[2], argv[3]);
            break;
        default:
            option_invalid_program_usage();
        break;
    }
    free_bmp_file(bmp);
    return 0;
}

void option_invalid_program_usage() {
    fprintf(stderr, "Invalid script usage\n"
                    "Correct syntaxes:\n"
                    "(1) bmpreader <file>\n"
                    "(2) bmpreader <input_file> <output_file>\n"
                    "(3) bmpreader <input_file> <output_file> <message_to_encrypt>\n");
    exit(1);
}