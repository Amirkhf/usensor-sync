#include "usensor.h"

static uint64_t ts_distance(uint64_t a, uint64_t b)
{
    if (a >= b)
        return (a - b);
    return (b - a);
}

static bool is_earlier(const t_record *a, const t_record *b)
{
    if (a->timestamp != b->timestamp)
        return (a->timestamp < b->timestamp);
    return (a->seq < b->seq);
}

t_record *select_camera(t_ring *camera, uint64_t button_ts)
{
    t_record *best = NULL;
    uint64_t best_dist = 0;

    for (size_t i = 0; i < camera->count; i++)
    {
        t_record *frame = &camera->data[i];
        uint64_t dist = ts_distance(frame->timestamp, button_ts);

        if (dist > CAMERA_MAX_NS)
            continue;
        if (best == NULL || dist < best_dist)
        {
            best = frame;
            best_dist = dist;
        }
        else if (dist == best_dist && is_earlier(frame, best))
            best = frame;
    }
    return (best);
}

bool imu_is_usable(const t_record *rec)
{
    return (rec->x >= 0 && rec->x <= YAW_MAX_CD);
}

t_record *select_imu(t_ring *imu, uint64_t button_ts)
{
    t_record *best = NULL;
    uint64_t best_dist = 0;

    for (size_t i = 0; i < imu->count; i++)
    {
        t_record *sample = &imu->data[i];
        uint64_t dist;

        if (!imu_is_usable(sample))
            continue;
        dist = ts_distance(sample->timestamp, button_ts);
        if (dist > IMU_MAX_NS)
            continue;
        if (best == NULL || dist < best_dist)
        {
            best = sample;
            best_dist = dist;
        }
        else if (dist == best_dist && is_earlier(sample, best))
            best = sample;
    }
    return (best);
}

t_record *select_gps(t_ring *gps, uint64_t button_ts)
{
    t_record *best = NULL;

    for (size_t i = 0; i < gps->count; i++)
    {
        t_record *fix = &gps->data[i];

        if (fix->timestamp > button_ts)
            continue;
        if (button_ts - fix->timestamp > GPS_MAX_AGE_NS)
            continue;
        if (best == NULL || is_earlier(best, fix))
            best = fix;
    }
    return (best);
}


static void insert_sorted(t_record *out, size_t count, const t_record *sample)
{
    size_t pos = count;

    while (pos > 0 && is_earlier(sample, &out[pos - 1]))
    {
        out[pos] = out[pos - 1];
        pos--;
    }
    out[pos] = *sample;
}

size_t collect_imu_window(t_ring *imu, uint64_t button_ts,
    t_record *out, size_t max)
{
    size_t count = 0;

    for (size_t i = 0; i < imu->count; i++)
    {
        t_record *sample = &imu->data[i];

        if (count == max)
            break;
        if (!imu_is_usable(sample))
            continue;
        if (ts_distance(sample->timestamp, button_ts) > IMU_WINDOW_NS)
            continue;
        insert_sorted(out, count, sample);
        count++;
    }
    return (count);
}
