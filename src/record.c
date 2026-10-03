#include "usensor.h"

bool parse_record(const uint8_t *raw, t_record *rec)
{
    if (u32_from_le_bytes(raw + 28) != fnv1a(raw, 28))
        return (false);
    if ((raw[13] & 1) == 0)
        return (false);
    rec->timestamp = u64_from_le_bytes(raw + 0);
    rec->seq = u32_from_le_bytes(raw + 8);
    rec->sensor_id = raw[12];
    rec->flags = raw[13];
    rec->x = (int32_t)u32_from_le_bytes(raw + 16);
    rec->y = (int32_t)u32_from_le_bytes(raw + 20);
    rec->z = (int32_t)u32_from_le_bytes(raw + 24);
    return (true);
}
