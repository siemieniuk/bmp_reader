# bmp_reader
This program reads parameters of BMP file, prints histogram, supports steganography and converts to the grayscale version.

## How to compile?
You can use GCC in order to compile program. Type the command:

    gcc main.c bmpin.c bmpout.c steganography.c -o bmpreader

## How to use the script?
Show details, histogram and steganography of the file:

    ./bmpreader <file.bmp>

Create grayscale of *in.bmp* and save to *out.bmp*:

    ./bmpreader <in.bmp> <out.bmp>

Create *out.bmp* file from *in.bmp* and embed steganography message into it:

    ./bmpreader <in.bmp> <out.bmp> <message_to_include>