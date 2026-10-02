#include <stdint.h>
#include <stddef.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <stdbool.h>

#define HEADER_SIZE 16
#define READ_OK 0
#define READ_EOF 1
#define READ_TRUNCATED 2
#define READ_ERROR 3



int read_exact(int fd, uint8_t *buf, size_t size)
{
    size_t total = 0;
    while (total != size)
    {
        ssize_t result = read(fd, buf + total, size - total);
        if (result == -1)
        {
            return (READ_ERROR);
        }
        else if (result == 0)
        {
            if (total == 0)
                return (READ_EOF);
            return (READ_TRUNCATED);
        }
        total += (size_t)result;
    }
    return (READ_OK);
}


bool has_good_signature(uint8_t *buf)
{
   const char good_signature[9] = "USENS001";
    for(int i = 0; i < 8;i++)
    {
        if(buf[i] != good_signature[i])
        {
            return(false);
        }
    }
    return(true);
}


uint16_t u16_from_le_bytes(const uint8_t *bytes)
{
    return ((uint16_t)(bytes[0] | (bytes[1] << 8)));
}


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
    else 
    {
       if(!has_good_signature(header))
        {
            return(false);
        }
        else if (u16_from_le_bytes(header + 8))
        {
            
        }
    }
    return (0);
}
