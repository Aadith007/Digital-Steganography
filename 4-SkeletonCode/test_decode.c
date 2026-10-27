
#include <stdio.h>
#include "decode.h"
#include "types.h"

int main(int argc, char *argv[])
{
    DecodeInfo decInfo;

    if (argc < 2 || argv[1][0] != '-' || argv[1][2] != '\0')
    {
        printf("Incorrect input format\n");
        return 1;
    }

    OperationType op = check_operation_type(argv[1][1]);

    if (op == e_decode)
    {
        if (read_and_validate_decode_args(argc, argv, &decInfo) == e_success)
        {
            printf("Decoding arguments validation success\n");

            if (do_decoding(&decInfo) == e_failure)
            {
                printf("Decoding failed\n");
                return 1;
            }

            printf("Decoding completed successfully\n");
        }
        else
        {
            printf("Decoding argument validation failed\n");
            return 1;
        }
    }
    
    else
    {
        printf("Unsupported operation\n");
        return 1;
    }

    return 0;
}
