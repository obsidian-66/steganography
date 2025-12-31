#include <stdio.h>
#include <string.h>
#include "encode.h"
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    //check operation type
    if(check_operation_type(argv) == e_encode)
        {
            printf("Encoding selected\n");
            EncodeInfo encInfo;
            if(read_and_validate_encode_args(argv, &encInfo) == e_success)
                {
                    printf("Read and validated input arguments successfully\n");
                    
                    if(do_encoding(&encInfo) == e_success)
                        {
                            printf("Encoding successful\n");
                        }
                    else
                        {
                            printf("Failed encoding\n");
                        }
                }
            else
                {
                    printf("Failed to validate\n");
                    printf("Syntax: <.out file> -e <base image .bmp> <secret file .txt> <output file name .txt>\n");
                }
        }
    else if(check_operation_type(argv) == e_decode)
        {
            printf("Decoding selected\n");
            DecodeInfo decInfo;
            if(read_and_validate_decode_args(argv, &decInfo) == e_success)
                {
                    printf("Read and validated decoding arguments successfully.\n");

                    if(do_decoding(&decInfo) == e_success)
                        {
                            printf("Decoding successful\n");
                        }
                    else
                        {
                            printf("Failed to decode\n");
                        }
                }
            else
                {
                    printf("Failed to validate\n");
                    printf("Syntax: <.out file> -d <encoded image .bmp>\n");
                }
        }
    else 
        {
            printf("Error. No such command found\n");
            printf("Usage info ..................");    //complete
        }

    //validation for files provided (mandatory and optional)


    return 0;
}

OperationType check_operation_type(char *argv[])
{
    if(strcmp(argv[1], "-e") == 0)
        return e_encode;
    else if(strcmp(argv[1], "-d") == 0)
        return e_decode;
    else
     return e_unsupported;
}