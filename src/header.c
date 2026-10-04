#include <stdio.h>
#include "usensor.h"

// verifie que l'en-tete commence par "usens001".
bool has_good_signature(const uint8_t *buf)
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


// verifie l'en-tete : signature taille de record version et checksum.
bool validate_header(const uint8_t *header)
{
    if (!has_good_signature(header))
    {
        fprintf(stderr, "error: invalid signature, expected \"USENS001\"\n");
        return (false);
    }
    if (u16_from_le_bytes(header + 8) != RECORD_SIZE)
    {
        fprintf(stderr, "error: invalid record size %u, expected %d\n",
            u16_from_le_bytes(header + 8), RECORD_SIZE);
        return (false);
    }
    if (u16_from_le_bytes(header + 10) != FORMAT_VERSION)
    {
        fprintf(stderr, "error: unsupported version %u, expected %d\n",
            u16_from_le_bytes(header + 10), FORMAT_VERSION);
        return (false);
    }
    if (u32_from_le_bytes(header + 12) != fnv1a(header, 12))
    {
        fprintf(stderr, "error: header checksum mismatch\n");
        return (false);
    }
    return (true);
}
