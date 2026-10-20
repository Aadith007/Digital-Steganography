#include <stdio.h>
#include "encode.h"
#include "types.h"

int main()
{
    EncodeInfo encInfo;
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
