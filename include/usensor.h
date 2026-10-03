#ifndef USENSOR_H
# define USENSOR_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>

# define HEADER_SIZE 16
# define RECORD_SIZE 32
# define FORMAT_VERSION 1

# define READ_OK 0
# define READ_EOF 1
# define READ_TRUNCATED 2
# define READ_ERROR 3

int         read_exact(int fd, uint8_t *buf, size_t size);

uint16_t    u16_from_le_bytes(const uint8_t *bytes);
uint32_t    u32_from_le_bytes(const uint8_t *bytes);
uint64_t    u64_from_le_bytes(const uint8_t *bytes);

uint32_t    fnv1a(const uint8_t *data, size_t len);

bool        has_good_signature(const uint8_t *buf);
bool        validate_header(const uint8_t *header);

#endif
