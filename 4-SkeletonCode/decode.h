
#ifndef DECODE_H
#define DECODE_H

#include "types.h"
#include <stdio.h>

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_DECODE_FILE_SUFFIX 5
#define MAX_OUTPUT_FILENAME 100

typedef struct _DecodeInfo
{
    
    char *src_image_fname;
    FILE *fptr_src_image;
    char image_data[MAX_IMAGE_BUF_SIZE];


    char secret_fname[20];
    FILE *fptr_secret;
    char extn_secret_file[MAX_DECODE_FILE_SUFFIX];
    long size_secret_file;

    
    char output_fname[100];
    FILE *fptr_output;

} DecodeInfo;

Status read_and_validate_decode_args(int argc, char *argv[], DecodeInfo *decInfo);

Status do_decoding(DecodeInfo *decInfo);

Status decode_magic_string(const char *magic_string, DecodeInfo *decInfo);

Status decode_secret_file_extn_size(uint *size, DecodeInfo *decInfo);

char decode_byte_from_lsb(char *image_buffer);

Status decode_secret_file_extn(uint size, DecodeInfo *decInfo);

#endif
