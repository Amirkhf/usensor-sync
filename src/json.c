#include "usensor.h"

static void emit_camera(t_ring *camera, uint64_t button_ts)
{
    const t_record *photo = select_camera(camera, button_ts);

    if (photo == NULL)
    {
        printf(",\"camera\":null");
        return ;
    }
    printf(",\"camera\":{\"timestamp_ns\":%llu", (unsigned long long)photo->timestamp);
    printf(",\"frame\":%lld}", (long long)photo->x);
}

static void emit_imu(t_ring *imu, uint64_t button_ts)
{
    const t_record *sample = select_imu(imu, button_ts);

    if (sample == NULL)
    {
        printf(",\"imu\":null");
        return ;
    }
    printf(",\"imu\":{\"timestamp_ns\":%llu", (unsigned long long)sample->timestamp);
    printf(",\"yaw_cd\":%lld", (long long)sample->x);
    printf(",\"pitch_cd\":%lld", (long long)sample->y);
    printf(",\"roll_cd\":%lld}", (long long)sample->z);
}

static void emit_gps(t_ring *gps, uint64_t button_ts)
{
    const t_record *fix = select_gps(gps, button_ts);

    if (fix == NULL)
    {
        printf(",\"gps\":null");
        return ;
    }
    printf(",\"gps\":{\"timestamp_ns\":%llu", (unsigned long long)fix->timestamp);
    printf(",\"lat_e7\":%lld", (long long)fix->x);
    printf(",\"lon_e7\":%lld", (long long)fix->y);
    printf(",\"alt_mm\":%lld}", (long long)fix->z);
}

static void emit_imu_window(t_ring *imu, uint64_t button_ts)
{
    static t_record window[RING_CAPACITY];
    size_t count = collect_imu_window(imu, button_ts, window, RING_CAPACITY);

    printf(",\"imu_window\":[");
    for (size_t i = 0; i < count; i++)
    {
        if (i > 0)
            printf(",");
        printf("{\"timestamp_ns\":%llu", (unsigned long long)window[i].timestamp);
        printf(",\"yaw_cd\":%lld", (long long)window[i].x);
        printf(",\"pitch_cd\":%lld", (long long)window[i].y);
        printf(",\"roll_cd\":%lld}", (long long)window[i].z);
    }
    printf("]");
}

// ecrit la ligne json complete d'un appui bouton.
void emit_event(t_record *bouton, t_ring *camera,t_ring *imu,t_ring *gps)
{
    printf("{\"event_id\":%llu", (unsigned long long)bouton->seq);
    printf(",\"timestamp_ns\":%llu", (unsigned long long)bouton->timestamp);
    emit_camera(camera, bouton->timestamp);
    emit_imu(imu, bouton->timestamp);
    emit_gps(gps, bouton->timestamp);
    emit_imu_window(imu, bouton->timestamp);
    printf("}\n");
}
