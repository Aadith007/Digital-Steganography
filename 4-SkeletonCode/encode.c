#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>

/* Function Definitions */

/* Get image size
 * Input: Image file ptr
 * Output: width * height * bytes per pixel (3 in our case)
 * Description: In BMP Image, width is stored in offset 18,
 * and height after that. size is 4 bytes
 */
uint get_image_size_for_bmp(FILE *fptr_image)
{
    uint width, height;
    // Seek to 18th byte
    fseek(fptr_image, 18, SEEK_SET);

    // Read the width (an int)
    fread(&width, sizeof(int), 1, fptr_image);
    printf("width = %u\n", width);

    // Read the height (an int)
    fread(&height, sizeof(int), 1, fptr_image);
    printf("height = %u\n", height);

    // Return image capacity
    return width * height * 3;
}

/* 
 * Get File pointers for i/p and o/p files
 * Inputs: Src Image file, Secret file and
 * Stego Image file
 * Output: FILE pointer for above files
 * Return Value: e_success or e_failure, on file errors
 */

 
Status read_and_validate_encode_args(int argc,char *argv[], EncodeInfo *encInfo){
    if((argc>5)||(argc<4)){
        printf("Input doest not contain the sufficient file/extra file is present\n");
        return e_failure;
    }
    int s_len=strlen(argv[2]);
    if (s_len<4){
    printf("Invalid source file\n");
    return e_failure;
    }
    if((argv[2][s_len-4]!='.')||(argv[2][s_len-3]!='b')||(argv[2][s_len-2]!='m')||(argv[2][s_len-1]!='p')){
        printf("Extension error in source file\n");
    return e_failure;
    }
    else{
        encInfo->src_image_fname=argv[2];
    }
    encInfo ->fptr_src_image=fopen(argv[2],"r"); 
    if(encInfo->fptr_src_image==NULL){
        printf("Source file not present\n");
        return e_failure;
    }
    //secretfile
    encInfo->secret_fname=argv[3];
    encInfo->fptr_secret=fopen(argv[3],"r");
    if(encInfo->fptr_secret==NULL){
        printf("Secret file not present\n");
        return e_failure;
    }

    //optional file

    if(argc>4){
    encInfo->stego_image_fname=argv[4];
    int steg_len=strlen(argv[4]);
        if(steg_len<4){
        printf("Invalid destination file\n");
        return e_failure;
        }
        if((argv[4][steg_len-4]!='.')||(argv[4][steg_len-3]!='b')||(argv[4][steg_len-2]!='m')||(argv[4][steg_len-1]!='p')){
             printf("Extension error in destination file\n");
            return e_failure;
        }
    }
    else
    encInfo->stego_image_fname="default.bmp";

     
        
        encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"w");
        if(encInfo->fptr_stego_image==NULL){
            printf("Destination file not present\n");
        return e_failure;
        }

    return e_success;
}
     status do_encoding(Encoding *encInfo){
        
          check_capacity(EncInfo)==e_failure
          print error msg,return e_failue

          ->copy_bmp_header(encInfo->fptr_src_mage,encInfo->fptr_stego_mage)==e_failure
          print error msg,return e_dfilur

          ->encode_magicstring(MAGIC STRING,encInfo)==e_failure
          print error msge , return e_failure

          ->char *dot=strchr(argv[3],'.')
          ->


        */
    

    status check_capacity(Encoding *encInfo){
        encInfo -> size_secret_file = get_file_size(encInfo->fptr_secret);
        if(encInfo->long size_secret_file ==0){
            printf("Secret file is empty\n");
            return e_failure;
        }
        encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
        long required_capacity =((long)encInfo->size_secret_file + 14) * 8;
  
        if(encInfo->image_capacity>=required_capacity){
            return e_success;
        }
        printf("Error: Insufficient image capacity\n");
        return e_failure;
    }



   uint get_file_size(FILE *fptr)
{
    if (fseek(fptr, 0, SEEK_END) != 0){
        return 0;
    }
    long res = ftell(fptr);
    if (res == -1L)
    {
         return 0;
    }
    return (uint)res;
}


    uint get_image_size_for_bmp(File *fptr_image){
        uint width;
        uint height;
        fseek(fptr_image,18,SEEK_SET);
        fread(&width,sizeof(uint),1,fptr_image);
        fseek(fptr_image,22,SEEK_SET);
        fread(&height,sizeof(uint),1,fptr_image);
        return width*height*3;

    }

    //Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
    //
      //      rewind(),src and scret file
        //    declare buff 54 byrs vhar buff[54];



    //

   // Status encode_magic_string(const char *magic_string, EncodeInfo *encInfo){

        //
          //  ->declare buff of 8 bytes
           // ->read 8 bytes from src_image into buff
           // ==>run loop for two length of magic string
            //->encode_byt_to_lsb(magic_string[],buff);
            //->write 8 bytes of encoded data frrom buff to dest_image

           // =>return status

        //
   // }
   // Status encode_byte_to_lsb(char data, char *image_buffer){
        
     //   ==>loop for 8 times
       // check nth bit set or not
         //   if set-> set lsb of image_buffer[]
          //  if clear -> clear lsb of image_buffer[]

        // ==>return e_success
    //}
    

//}



    










    



