#include "usensor.h"

void ring_push(const t_record *rec, t_ring *ring)
{
    ring->data[ring->head] = *rec;
    if(ring->count < RING_CAPACITY)
        ring->count += 1;
    ring->head += 1;
    if(ring->head == RING_CAPACITY)
        ring->head = 0;
}