# bmp_reader
This program reads parameters of BMP file, prints histogram, converts to the grayscale version and has support for steganography.

## How to compile?
You can use GCC or any other C compiler. In case of GCC type the command:

    gcc main.c bmpin.c bmpout.c steganography.c -o bmpreader

## How to use this program?

Once you compiled a program, you are able to use it in practice.

Show details, histogram and steganography (if exists) of the file:

    ./bmpreader <file.bmp>

Create a grayscale version of *in.bmp* and save the result to *out.bmp*:

    ./bmpreader <in.bmp> <out.bmp>

Create *out.bmp* file from *in.bmp* and embed a steganography message inside:

    ./bmpreader <in.bmp> <out.bmp> <message_to_include>