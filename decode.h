#ifndef DECODE_H
#define DECODE_H

#include "types.h"

#define MAX_SECRET_BUF_SIZE 1
#define MAX_IMAGE_BUF_SIZE (MAX_SECRET_BUF_SIZE * 8)
#define MAX_FILE_SUFFIX 4

typedef struct _decodeInfo
{
    /*Stego image info*/
    char *stego_image_fname;
    FILE *fptr_stego_image;

    /*Output file info*/
    char *output_fname;     //base name without extension
    FILE *fptr_output;

    /*Decoded secret info*/
    char extn_secret_file[MAX_FILE_SUFFIX];
    uint extn_size;
    uint size_secret_file;

    /*Buffers*/
    char image_data[MAX_IMAGE_BUF_SIZE];
    char secret_data[MAX_SECRET_BUF_SIZE];
}DecodeInfo;

Status do_decoding(DecodeInfo *decInfo);

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo);

Status open_files_dec(DecodeInfo *decInfo);

Status decode_magic_string(DecodeInfo *decInfo);

Status decode_lsb_to_byte(char *image_buffer, char *data);

Status decode_lsb_to_size(char *image_buffer, uint *size);

Status decode_secret_file_extn_size(DecodeInfo *decInfo);

Status decode_secret_file_extn(DecodeInfo *decInfo);

Status decode_secret_file_size(DecodeInfo *decInfo);

Status decode_secret_file_data(DecodeInfo *decInfo);

#endif