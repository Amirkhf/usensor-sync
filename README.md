# usensor

My program reads the sensor data sent by `usensor`. On every button press, it writes a JSON line with the photo, the IMU and the GPS fix from that same moment.

## Run

```sh
make
./usensor --seed 42 --duration 300s | ./<your_binary>
```

## How it works

1. I check the header (signature, size, version, checksum). If it is wrong, the program stops.
2. I read the 32-byte records one by one, and turn the bytes into numbers (little-endian).
3. If a record's checksum or flag is wrong, I skip it.
4. I store the camera, imu and gps samples in 3 arrays.
5. For each button press, I wait 102 ms, then I write the JSON with `printf`.

## Memory

- A circular buffer of 512 slots per sensor, no `malloc`: once it is full, the new sample replaces the oldest one. Memory never grows.
- 512 is enough: I look back at most 1.102 s and there are at most 400 records per second (1.102 x 400 ≈ 441).
- Pending button presses sit in an array of 16 slots (at most 1 press per second).

## Late records

A record can arrive up to 2 ms late. I keep `max_ts`, the largest timestamp seen so far. A button at time T is written once `max_ts` goes past T + 102 ms (100 ms imu window + 2 ms of lateness). At the end of the stream, I write the remaining buttons.

## Errors

- Wrong or truncated header, truncated last record, `read()` error: error message, the program returns 1.
- Wrong record: skipped.
- Too many pending buttons: the press is skipped with a warning.

## What I assume

- At most 400 records per second and 1 button press per second, at most 2 ms of lateness (this is given in the subject).
- GPS: if two fixes share the same timestamp, I keep the one with the largest `seq`.
