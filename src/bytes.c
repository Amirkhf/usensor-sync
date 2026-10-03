#include "usensor.h"

uint16_t u16_from_le_bytes(const uint8_t *bytes)
{
    return ((uint16_t)(bytes[0] | (bytes[1] << 8)));
}


uint32_t u32_from_le_bytes(const uint8_t *bytes)
{
    return ((uint32_t)bytes[0]
        | ((uint32_t)bytes[1] << 8)
        | ((uint32_t)bytes[2] << 16)
        | ((uint32_t)bytes[3] << 24));
}


uint64_t u64_from_le_bytes(const uint8_t *bytes)
{
    return ((uint64_t)bytes[0]
        | ((uint64_t)bytes[1] << 8)
        | ((uint64_t)bytes[2] << 16)
        | ((uint64_t)bytes[3] << 24)
        | ((uint64_t)bytes[4] << 32)
        | ((uint64_t)bytes[5] << 40)
        | ((uint64_t)bytes[6] << 48)
        | ((uint64_t)bytes[7] << 56));
}
