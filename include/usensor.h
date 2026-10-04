#ifndef USENSOR_H
# define USENSOR_H

# include <stdint.h>
# include <stddef.h>
# include <stdbool.h>
# include <stdio.h>
# include <inttypes.h>

# define HEADER_SIZE 16
# define RECORD_SIZE 32
# define FORMAT_VERSION 1
# define RING_CAPACITY 512
# define MAX_PENDING 16
# define FINALIZE_DELAY_NS 102000000
# define CAMERA_MAX_NS 50000000
# define IMU_MAX_NS 20000000
# define GPS_MAX_AGE_NS 1000000000
# define YAW_MAX_CD 35999
# define READ_OK 0
# define READ_EOF 1
# define READ_TRUNCATED 2
# define READ_ERROR 3
# define IMU_WINDOW_NS 100000000

typedef struct s_record
{
   uint64_t timestamp;
   uint32_t seq;
   uint8_t sensor_id;
   uint8_t flags;
   int32_t x;
   int32_t y;
   int32_t z;
} t_record;

typedef struct s_ring
{
    t_record data[RING_CAPACITY];
    size_t  head;
    size_t  count;
}   t_ring;


int         read_exact(int fd, uint8_t *buf, size_t size);

uint16_t    u16_from_le_bytes(const uint8_t *bytes);
uint32_t    u32_from_le_bytes(const uint8_t *bytes);
uint64_t    u64_from_le_bytes(const uint8_t *bytes);

uint32_t    fnv1a(const uint8_t *data, size_t len);

bool        has_good_signature(const uint8_t *buf);
bool        validate_header(const uint8_t *header);

bool        parse_record(const uint8_t *raw, t_record *rec);
void ring_push(const t_record *rec, t_ring *ring);
t_record    *select_camera(t_ring *camera, uint64_t button_ts);
bool        imu_is_usable(const t_record *rec);
t_record    *select_imu(t_ring *imu, uint64_t button_ts);
t_record    *select_gps(t_ring *gps, uint64_t button_ts);
size_t      collect_imu_window(t_ring *imu, uint64_t button_ts,
                t_record *out, size_t max);
void emit_event(t_record *bouton, t_ring *camera,t_ring *imu,t_ring *gps);

#endif
