#include "usensor.h"

void emit_event(t_record *bouton, t_ring *camera,t_ring *imu,t_ring *gps)
{
    (void)camera;
    (void)imu;
    (void)gps;
    printf("{\"event_id\":%llu", (unsigned long long)bouton->seq);
    printf(",\"timestamp_ns\":%llu", (unsigned long long)bouton->timestamp);
    printf("}\n");
}
