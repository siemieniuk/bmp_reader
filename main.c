#include <stdio.h>
#include <stdlib.h>
#include "bmpall.h"

int main(int argc, char* argv[]) {
    if (argc == 1 || argc >= 4) {
        fprintf(stderr, "Invalid script usage\n"
                        "Correct syntaxes:\n"
                        "(1) bmpreader <file>\n"
                        "(2) bmpreader <input_file> <output_file>\n");
        exit(1);
    }
    BMPfile* bmp = read_bmp_file(argv[1]);
    print_bitmap_file_header(bmp->file_header);
    print_bitmap_info_header(bmp->info_header);
    // check if output path was specified
    if (argc == 3) {
        // TODO: process with two paths
    }
    free_bmp_file(bmp);
    return 0;
}