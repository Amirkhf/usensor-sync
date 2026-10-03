#include <stdio.h>
#include "usensor.h"

int main(void)
{
    uint8_t header[HEADER_SIZE];
    uint8_t record[RECORD_SIZE];

    int status = read_exact(0, header, HEADER_SIZE);
    if (status != READ_OK)
    {
        if (status == READ_ERROR)
            perror("error: failed to read input");
        else if (status == READ_TRUNCATED)
            fprintf(stderr, "error: truncated header\n");
        else if (status == READ_EOF)
            fprintf(stderr, "error: empty input, no header\n");
        return (1);
    }
    if (!validate_header(header))
        return (1);
    t_record rec;
    while (1)
    {
        status = read_exact(0, record,RECORD_SIZE);
         if (status == READ_ERROR)
         {
             perror("error: failed to read input");
            return(1);
         }
        else if (status == READ_TRUNCATED)
        {
            fprintf(stderr, "error: truncated header\n");
            return(1);
        }   
        else if (status == READ_EOF)
            break;
        if (!parse_record(record, &rec))
            continue;
            
    }
    
    return (0);
}
