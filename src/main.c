#include <stdio.h>
#include "usensor.h"

int main(void)
{
    uint8_t header[HEADER_SIZE];

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
    return (0);
}
