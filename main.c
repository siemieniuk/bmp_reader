#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    if (argc == 1 || argc >= 4) {
        fprintf(stderr, "Invalid script usage\n"
                        "Correct syntaxes:\n"
                        "(1) bmpreader <file>\n"
                        "(2) bmpreader <input_file> <output_file>\n");
        exit(1);
    } else if (argc == 2) {
        // TODO: process with one path
    } else { //argc == 3
        // TODO: process with two paths
    }
    return 0;
}