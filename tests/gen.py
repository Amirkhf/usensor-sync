#!/usr/bin/env python3
"""Génère un flux USENS001 de test sur stdout.
Usage: python3 tests/gen.py > tests/buttons.bin"""
import struct, sys

def fnv1a(data):
    h = 0x811C9DC5
    for b in data:
        h = ((h ^ b) * 0x01000193) & 0xFFFFFFFF
    return h

def header():
    h = b"USENS001" + struct.pack("<HH", 32, 1)
    return h + struct.pack("<I", fnv1a(h))

def record(ts, seq, sensor, x=0, y=0, z=0, flags=1, bad_crc=False):
    r = struct.pack("<QIBBHiii", ts, seq, sensor, flags, 0, x, y, z)
    crc = fnv1a(r) ^ (1 if bad_crc else 0)
    return r + struct.pack("<I", crc)

MS = 1_000_000
out = header()
out += record(0 * MS, 0, 5, x=1)                 # appui bouton à t=0
out += record(50 * MS, 1, 2)                     # imu, pas encore finalisé
out += record(60 * MS, 2, 5, x=0)                # relâchement : ignoré
out += record(150 * MS, 3, 2)                    # > 102 ms → bouton seq=0 affiché
out += record(160 * MS, 4, 5, x=1, bad_crc=True) # checksum faux : ignoré
out += record(170 * MS, 5, 5, x=1, flags=0)      # flag valide à 0 : ignoré
out += record(200 * MS, 6, 5, x=1)               # appui en fin de flux → vidé à EOF
sys.stdout.buffer.write(out)
