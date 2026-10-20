#include <stdio.h>
#include "encode.h"
#include "types.h"

int main(int argc,char *argv[])
{
    EncodeInfo encInfo;
    if(argc<2 ||argv[1][0]!='-'||argv[1][2]!='\0'){
        printf("Usage: ./a.out -e <source.bmp> <message.txt> [output.bmp]\n");
        return 1;
    }
    if(check_operation_type(argv[1][1])==e_encode){
        read_and_validate_encode_args(argv,&encInfo);
    }
    //else if(check_operation_type(argv[1][1])==e_decode)
    //read_and_validate_decode_args(argv, DecodeInfo *decInfo);
    else
    printf("Un supported operation\n");
    
   // (call check_operation_type(argv[1][1])==e_encode)
   //read_and_validate_encode_args(argv, EncodeInfo *encInfo);
   //
    return 0;
}
OperationType check_operation_type(char opt){
    if(opt=='e')
    return e_encode;
    if(opt=='d')
    return e_decode;
    else
    return e_unsupported;
}
