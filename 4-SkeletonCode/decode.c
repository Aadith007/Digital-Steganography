#include<stdio.h>
#include<string.h>
#include "decode.h"
#include "common.h"

Status read_and_validate_decode_args(int argc,char *argv[],DecodeInfo *decInfo){
    if((argc<3) ||(argc>4)){
        printf("Insufficient command line args\n");
        return e_failure;
    }
    int len=strlen(argv[2]);
    if(len<4){
    printf("Invalid source file\n");
    return e_failure;
}
if((argv[2][len-4]!='.')||(argv[2][len-3]!='b')||(argv[2][len-2]!='m')||(argv[2][len-1]!='p')){
        printf("Extension error in source file\n");
    return e_failure;
    }
    decInfo->src_image_fname=argv[2];
     decInfo->fptr_src_image = fopen(decInfo->src_image_fname, "rb");
    if(decInfo->fptr_src_image==NULL){
        printf("Error:Cant open stego image\n");
        return e_failure;
    }

    if(argc>3){
       strcpy(decInfo->output_fname,argv[3]);
    }
    else{
       strcpy(decInfo->output_fname,"decoded");
    }
    return e_success;
}
Status do_decoding(DecodeInfo *decInfo){
    if(decode_magic_string(MAGIC_STRING,decInfo)==e_failure){
        printf("Error: Failed to decode magic string\n");
        return e_failure;
    }
    uint extn_size;
    if(decode_secret_file_extn_size(&extn_size,decInfo)==e_failure){
        printf("Error: Failed to decode secret file extn size\n");
        return e_failure;
    }
    if(decode_secret_file_extn(extn_size,decInfo)==e_failure){
        printf("Error: Failed to decode secret file extn\n");

        return e_failure;
    }
    
    if(decode_secret_data_size(decInfo)==e_failure){
        printf("Error: Failed to decode secret data size\n");
        return e_failure;
    }
    if(decode_secret_file_data(decInfo)==e_failure){
        printf("Error: Failed to decode secret data\n");
        return e_failure;
    }

    return e_success;
}




Status decode_magic_string(const char *magic_string,DecodeInfo *decInfo){
    char buff[8];
    int len=strlen(MAGIC_STRING);
    fseek(decInfo->fptr_src_image, 54, SEEK_SET);

    for(int i=0;i<len;i++){
        if(fread(buff,1,8,decInfo->fptr_src_image)!=8){
            return e_failure;
        }
        if(decode_byte_from_lsb(buff)!=magic_string[i]){
            return e_failure;
        }
        if(decode_byte_from_lsb(buff)!=magic_string[i]){
            return e_failure;
        }
       

    }
     return e_success;
           
}

char decode_byte_from_lsb(char *image_buffer){
    char data=0;
    for(int i=0;i<8;i++){
        data=(data<<1)|(image_buffer[i]&1);
    }
    return data;
}
Status decode_secret_file_extn_size(uint *size,DecodeInfo *decInfo){
    char buff[32];
    if(fread(buff,1,32,decInfo->fptr_src_image)!=32){
        printf("Error: Incomlete output image\n");
        return e_failure;
    }

    uint data=0;
    for(int i=0;i<32;i++){
        data=(data<<1)|(buff[i]&1);

    }
    *size=data;
    printf("Decoded extension size: %u\n", *size);
    return e_success;

}
Status decode_secret_file_extn(uint size, DecodeInfo *decInfo)
{
    char buff[8];

    for (int i = 0; i < size; i++)
    {
        if (fread(buff, 1, 8, decInfo->fptr_src_image) != 8)
            return e_failure;

        decInfo->extn_secret_file[i] = decode_byte_from_lsb(buff);

       
    }

    decInfo->extn_secret_file[size] = '\0';

    printf("Decoded extension: %s\n", decInfo->extn_secret_file);

    strcat(decInfo->output_fname, decInfo->extn_secret_file);
    printf("%s\n",decInfo->output_fname);

    return e_success;
}
Status decode_secret_data_size(DecodeInfo *decInfo){
   
    char buff[32];
    if(fread(buff,1,32,decInfo->fptr_src_image)!=32){
        printf("Error: Failed to read 32 bytes from source image");
        return e_failure;
    }
    uint size=0;
    for(int i=0;i<32;i++){
        size=(size<<1)|(buff[i]&1);

    }
     decInfo->size_secret_file = size;
    printf("Decoded secret file size: %u bytes\n", size);

    decInfo->fptr_output = fopen(decInfo->output_fname, "wb");

if (decInfo->fptr_output == NULL){
    printf("Error: Cannot create output file\n");
    return e_failure;
}

    return e_success;
}

Status decode_secret_file_data(DecodeInfo *decInfo){
    char buff[8];
    char data;
    for(int i=0;i< decInfo->size_secret_file;i++){
        if(fread(buff,1,8,decInfo->fptr_src_image)!=8){
            printf("Error: Failed to read 8 bytes from source image\n");
            return e_failure;
        }
        data=decode_byte_from_lsb(buff);
        if(fwrite(&data,1,1,decInfo->fptr_output)!=1){
            printf("Error: Failed to write the secret data to output file\n");
            return e_failure;
        }
    }
    fclose(decInfo->fptr_output);
    return e_success;
}


