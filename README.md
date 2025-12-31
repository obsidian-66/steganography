COMMON.h:
  Contains a MAGIC_STRING
  It is used as a vaildation to check if the given encoded file actually contains any data or not.
  The code checks for the encoded magic string first, thereby reducing the execution time in case of no encoding.

encode.c/decode.c:
  Contains the corresponding function definitions.
  Include the function declarations in .h files

  test_encode.c:
    Contains the main file.
    Execution starts here.

HOW TO USE:
  1.  Inputs are using command line arguments(CLA).
  2.  Encoding syntax:
      <.out file> -e <image .bmp> <secret file .txt> <output file .txt (not mandatory)>
  3.  Decoding syntax:
      <.out file> -d <encoded image .bmp> <decoded file .txt (not mandatory)>

NOTE:
  beautiful.bmp and stego.bmp are included in the folder. You may replace the image with your choice. Please note that it should be a .bmp file.
  secret.txt file contains the secret message. It can also be replaced as necessary.
  stego.bmp is the encoded file. If beautiful.bmp is replaced, the encoded file will look different.
  output.txt contains the decode data.
