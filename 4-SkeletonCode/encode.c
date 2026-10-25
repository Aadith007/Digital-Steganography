#include <stdio.h>
#include "encode.h"
#include "types.h"
#include<string.h>
#include"common.h"

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

     
        
        encInfo->fptr_stego_image=fopen(encInfo->stego_image_fname,"wb");
        if(encInfo->fptr_stego_image==NULL){
            printf("Destination file not present\n");
        return e_failure;
        }

    return e_success;
}
     Status do_encoding(EncodeInfo *encInfo){
        
        if(check_capacity(encInfo)==e_failure){
            printf("Error: Insufficient image capacity\n");
            return e_failure;
          }


        if(copy_bmp_header(encInfo->fptr_src_image,encInfo->fptr_stego_image)==e_failure){

            printf("Error:Failed to copy BMP header\n");
            return e_failure;
          }
          

        if (encode_magic_string(MAGIC_STRING, encInfo) == e_failure)
            {
                printf("Error: Failed to encode magic string\n");
                 return e_failure;
            }
          char *dot=strchr(encInfo->secret_fname,'.');
        if(dot==NULL){
            printf("Error:File extn not found\n");
            return e_failure;
            }
          strcpy(encInfo->extn_secret_file,dot);

        if(encode_secret_file_extn_size(strlen(encInfo->extn_secret_file),encInfo)==e_failure){
            printf("Error: extension length mismatch\n");
            return e_failure;
        }
        if(encode_secret_file_extn((encInfo->extn_secret_file),encInfo)==e_failure){
            printf("Error: Failed to encode secret file extension size\n");
            return e_failure;
        }
        if(encode_secret_file_size(encInfo->size_secret_file,encInfo)==e_failure){
            printf("Error: Failed to encode secret file size");
            return e_failure;
        }

          return e_success;
        }
        
    

    Status check_capacity(EncodeInfo *encInfo){
        encInfo -> size_secret_file = get_file_size(encInfo->fptr_secret);
        if(encInfo->size_secret_file ==0){
            printf("Secret file is empty\n");
            return e_failure;
        }
        encInfo->image_capacity=get_image_size_for_bmp(encInfo->fptr_src_image);
        long required_capacity =((long)encInfo->size_secret_file + 14) * 8;
  
        if(encInfo->image_capacity>=required_capacity){
            return e_success;
        }
        
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
    rewind(fptr);
    return (uint)res;
}

    Status copy_bmp_header(FILE *fptr_src_image, FILE *fptr_dest_image){
        char buff[54];

        rewind(fptr_src_image);
        rewind(fptr_dest_image);
        if(fread(buff,1,54,fptr_src_image)!=54){
            printf("Incomplete header file of source image");
            return e_failure;
        }

         if(fwrite(buff,1,54,fptr_dest_image)!=54){
            return e_failure;
        }
        return e_success;


    } 
      
    Status encode_magic_string(const char *magic_string,EncodeInfo *encInfo){
        char buff[8];
        int len=strlen(MAGIC_STRING);
        for(int i=0;i<len;i++){
            if(fread(buff,1,8,encInfo->fptr_src_image)!=8){
                return e_failure;
            }
            if(encode_byte_to_lsb(magic_string[i],buff)==e_success)
            {
                
                if(fwrite(buff,1,8,encInfo->fptr_stego_image)!=8)
                {
                return e_failure;
                }
            }
            else
            {
            return e_failure;
            }

        }
        return e_success;
    }

    Status encode_byte_to_lsb(char data,char *image_buffer){
        
        for(int i=0;i<8;i++)
        {
            int mask=1<<(7-i);
        // // to check nth bit is set of secret data
        if(data&mask){ // it is set
            // set image buffer lsb
            image_buffer[i]=image_buffer[i]|1;
        }
        else{
            image_buffer[i]=image_buffer[i]&(~1);
        }
            
        }   
    return e_success;
    }

    Status encode_secret_file_extn_size(uint size,EncodeInfo *encinfo){
        char buff[32];
        if(fread(buff,1,32,encinfo->fptr_src_image)!=32){
                printf("Error: incomplete source image\n");
                return e_failure;
            }
            if(encode_size_to_lsb(size,buff)!=e_success){
                return e_failure;
            }
            if(fwrite(buff,1,32,encinfo->fptr_stego_image)!=32){
                return e_failure;
            }
            return e_success;
        }

    Status encode_size_to_lsb(uint size,char *image_buffer){
        for(int i=0;i<32;i++){
            uint mask=1U<<(31-i);
            if(size&mask){
                image_buffer[i]=image_buffer[i]|1;
            }
            else{
            image_buffer[i]=image_buffer[i]&(~1);
            }
        }
        return e_success;

    }

    Status encode_secret_file_extn(const char *file_extn,EncodeInfo *encinfo){
        char buff[8];
        int len=strlen(file_extn);
        for(int i=0;i<len;i++){
            if(fread(buff,1,8,encinfo->fptr_src_image)!=8){
                printf("Error: Failed to read source image\n");
                return e_failure;
            }
            if(encode_byte_to_lsb(file_extn[i],buff)!=e_success){
                printf("Error: Encoding file_extn failed\n");
                return e_failure;
            }
            if(fwrite(buff,1,8,encinfo->fptr_stego_image)!=8){
                printf("Error: Failed to write extension to the destination fille\n");
                return e_failure;

            }
        }
        return e_success;
    }

    Status encode_secret_file_size(long file_size,EncodeInfo *encInfo){
        char buff[32];
        if(fread(buff,1,32,encInfo->fptr_src_image)!=32){
            printf("Error: Failed to to read source image\n");
            return e_failure;
        }
        if(encode_size_to_lsb(file_size,buff)!=e_success){
            printf("Error: Encoding the secret file size failed\n");
            return e_failure;
        }
        if(fwrite(buff,1,32,encInfo->fptr_stego_image)!=32){
            printf("Error: Failed to write secret file size to destination image\n");
            return e_failure;
        }
        return e_success;
    }

    Status encode_data_to_image(char *data,int size,FILE *fptr_src_image,FILE *fptr_stego_image){
        char buff[8];
        for(int i=0;i<size;i++){
            if(fread(buff,1,8,fptr_src_image)!=8){
                printf("Error: failed to read source image\n");
                return e_failure;
            }
            if(encode_byte_to_lsb(data,buff)!=e_success){
                printf("Error: Failed to encode")
            }

        }
    }



    










    



