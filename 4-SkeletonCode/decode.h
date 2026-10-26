
#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include <stdio.h>

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _DecodeInfo
{
    /* Stego Image Info */
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /* Secret File Info */
    char extn_secret_file[MAX_FILE_SUFFIX];
    long size_secret_file;

    /* Output File Info */
    char *output_fname;
    FILE *fptr_output;

} DecodeInfo;


Status read_and_validate_decode_args(int argc, char *argv[],DecodeInfo *decInfo);
Status do_decoding(DecodeInfo *decInfo);
Status decode_magic_string(const char *magic_string, EncodeInfo *encInfo);


#endif
