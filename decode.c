#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "decode.h"
#include "types.h"
#include "common.h"

Status read_and_validate_decode_args(char *argv[], DecodeInfo *decInfo)
{
    //Validate stego image .bmp
    if(argv[2] != NULL && strstr(argv[2], ".") != NULL && strcmp(strstr(argv[2], "."), ".bmp") == 0)
        {
            decInfo->stego_image_fname = argv[2];
        }
    else
        {
            printf("ERROR: Invalid stego image file.\n");
            return e_failure;
        }
    
    //Allocate memory for output file name
    decInfo->output_fname = malloc(100);
    if(decInfo->output_fname == NULL)
        {
            printf("ERROR: Memory allocation failed.\n");
            return e_failure;
        }
    
    //Optional Output file name
    if(argv[3] != NULL)
        {
            strcpy(decInfo->output_fname, argv[3]);
        }
    else    
        {
            strcpy(decInfo->output_fname, "output");
        }
    
    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo)
{
    char ch;

    for(uint i=0; i<decInfo->size_secret_file; i++)
        {
            fread(decInfo->image_data, 8, 1, decInfo->fptr_stego_image);
            decode_lsb_to_byte(decInfo->image_data, &ch);
            fwrite(&ch, 1, 1, decInfo->fptr_output);
        }

    return e_success;
}

Status decode_secret_file_size(DecodeInfo *decInfo)
{
    fread(decInfo->image_data, 32, 1, decInfo->fptr_stego_image);
    decode_lsb_to_size(decInfo->image_data, &decInfo->size_secret_file);

    return e_success;
}

Status decode_secret_file_extn(DecodeInfo *decInfo)
{
    char ch;

    for(uint i=0; i<decInfo->extn_size; i++)
        {
            fread(decInfo->image_data, 8, 1, decInfo->fptr_stego_image);
            decode_lsb_to_byte(decInfo->image_data, &ch);
            decInfo->extn_secret_file[i] = ch;
        }
    decInfo->extn_secret_file[decInfo->extn_size] = '\0';

    return e_success;
}

Status decode_lsb_to_size(char *image_buffer, uint *size)
{
    *size = 0;
    
    for(int i=0; i<32; i++)
        {
            *size |= (image_buffer[i] & 1) << (31 - i);
        }
    return e_success;
}

Status decode_secret_file_extn_size(DecodeInfo *decInfo)
{
    fread(decInfo->image_data, 32, 1, decInfo->fptr_stego_image);
    decode_lsb_to_size(decInfo->image_data, &decInfo->extn_size);
    return e_success;
}

Status decode_lsb_to_byte(char *image_buffer, char *data)
{
    *data = 0;

    for(int i=0; i<8; i++)
        {
            *data |= (image_buffer[i] & 1) << (7 - i);
        }
    return e_success;
}

Status decode_magic_string(DecodeInfo *decInfo)
{
    char ch;

    for(int i=0; i<strlen(MAGIC_STRING); i++)
        {
            fread(decInfo->image_data, 8, 1, decInfo->fptr_stego_image);
            decode_lsb_to_byte(decInfo->image_data, &ch);

            if(ch != MAGIC_STRING[i])
                {
                    printf("ERROR: Magic string mismatch.\n");
                    return e_failure;
                }
        }
    return e_success;
}

Status do_decoding(DecodeInfo *decInfo)
{
    //Open stego image
    decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if(decInfo->fptr_stego_image == NULL)
        {
            perror("fopen");
            return e_failure;
        }

    //Skip header bits
    fseek(decInfo->fptr_stego_image, 54, SEEK_SET);

    //Decode magic string
    if(decode_magic_string(decInfo) != e_success)
        {
            printf("Magic string not found.\n");
            return e_failure;
        }

    //Decode extension size 
    if(decode_secret_file_extn_size(decInfo) != e_success)
        {
            printf("Error decoding file extension size.\n");
            return e_failure;
        }

    //Decode extension
    if(decode_secret_file_extn(decInfo) != e_success)
        {
            printf("Error decoding secret file extension.\n");
            return e_failure;
        }

    //Create output file with decoded extension
    strcat(decInfo->output_fname, decInfo->extn_secret_file);
    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");
    if(decInfo->fptr_output == NULL)
        {
            perror("fopen");
            return e_failure;
        }

    //Decode secret file size
    if(decode_secret_file_size(decInfo) != e_success)
        {
            printf("Failed to decode secret file size.\n");
            return e_failure;
        }

    //Decode secret file data
    if(decode_secret_file_data(decInfo) != e_success)
        {
            printf("Failed to decode secret file.\n");
            return e_failure;
        }
    
        fclose(decInfo->fptr_output);
        fclose(decInfo->fptr_stego_image);

        printf("Decoding successful. Output file: %s\n", decInfo->output_fname);

        return e_success;
}