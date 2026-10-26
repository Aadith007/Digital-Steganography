#include<stdio.h>
#include<string.h>
#include "decode.h"
#include<common.h>

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
    decInfo->stego_image_fname=argv[2];
     decInfo->fptr_stego_image = fopen(decInfo->stego_image_fname, "rb");
    if(decInfo->fptr_stego_image==NULL){
        printf("Error:Cant open stego image\n");
        return e_failure;
    }

    if(argc>3){
        decInfo->output_fname=argv[3];
        int out_len=strlen(argv[3]);
        if(out_len<4){
            printf("Invalid destination file\n");
            return e_failure;
        }
        if((argv[3][out_len-4]!='.')||(argv[3][out_len-3]!='t')||(argv[3][out_len-2]!='x')||(argv[3][out_len-1]!='t')){
             printf("Extension error in destination file\n");
            return e_failure;
        }
        

    }
    else{
        decInfo->output_fname="decoded.txt";
    }
    return e_success;
}

