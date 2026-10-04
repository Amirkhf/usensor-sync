#include <unistd.h>
#include "usensor.h"

// lit exactement size octets meme si read() en renvoie moins d'un coup.
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
