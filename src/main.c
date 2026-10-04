#include <stdio.h>
#include <inttypes.h>
#include "usensor.h"

// verifie l'en-tete puis lit les records un par un.
// un bouton est ecrit en json 102 ms apres son timestamp.
// les boutons restants sont ecrits a la fin du flux.
int main(void)
{
    uint8_t header[HEADER_SIZE];
    uint8_t record[RECORD_SIZE];
    t_ring  camera_ring = {0};
    t_ring  imu_ring = {0};
    t_ring  gps_ring = {0};
    t_record pending[MAX_PENDING];
    size_t pending_count  = 0;
    uint64_t max_ts = 0;

    int status = read_exact(0, header, HEADER_SIZE);
    if (status != READ_OK)
    {
        if (status == READ_ERROR)
            perror("error: failed to read input");
        else if (status == READ_TRUNCATED)
            fprintf(stderr, "error: truncated header\n");
        else if (status == READ_EOF)
            fprintf(stderr, "error: empty input, no header\n");
        return (1);
    }
    if (!validate_header(header))
        return (1);
    t_record rec;
    while (1)
    {
        status = read_exact(0, record,RECORD_SIZE);
         if (status == READ_ERROR)
         {
             perror("error: failed to read input");
            return(1);
         }
        else if (status == READ_TRUNCATED)
        {
            fprintf(stderr, "error: truncated record\n");
            return(1);
        }
        else if (status == READ_EOF)
            break;
        if (!parse_record(record, &rec))
            continue;
        if (rec.timestamp > max_ts)
            max_ts = rec.timestamp;
       if (rec.sensor_id == 1) // camera
            ring_push(&rec,&camera_ring);
       else if (rec.sensor_id == 2) // imu
            ring_push(&rec,&imu_ring);
       else if (rec.sensor_id == 3) // gps
            ring_push(&rec,&gps_ring);
       else if (rec.sensor_id == 5) // button
       {
            if (rec.x == 1)
            {
                if (pending_count < MAX_PENDING)
                {
                    pending[pending_count] = rec;
                    pending_count++;
                }
                else
                    fprintf(stderr, "warning: too many pending button presses, dropped\n");
            }
       }
       size_t i = 0;
       while (i < pending_count)
       {
            if (max_ts > pending[i].timestamp + FINALIZE_DELAY_NS)
            {
                emit_event(&pending[i], &camera_ring, &imu_ring, &gps_ring);
                for (size_t j = i; j + 1 < pending_count; j++)
                    pending[j] = pending[j + 1];
                pending_count--;
            }
            else
                i++;
       }

    }
    size_t i = 0;
    while (i < pending_count)
    {
        emit_event(&pending[i], &camera_ring, &imu_ring, &gps_ring);
        i++;
    }
    pending_count = 0;
    return (0);
}
