#!/usr/bin/env python3
import struct, zlib, sys, math

W, H = 1024, 768
out = sys.argv[1]

def chunk(kind, data):
    return struct.pack(">I", len(data)) + kind + data + struct.pack(">I", zlib.crc32(kind + data) & 0xffffffff)

rows = []
for y in range(H):
    row = bytearray([0])
    for x in range(W):
        t = y / (H - 1)
        u = x / (W - 1)
        r = int(7 + 9*t)
        g = int(13 + 18*t)
        b = int(28 + 38*t)
        # soft cyan/blue glows
        d1 = math.hypot(x-800, y-80)
        d2 = math.hypot(x-120, y-700)
        a1 = max(0, 1-d1/360)
        a2 = max(0, 1-d2/500)
        r += int(0*a1 + 12*a2)
        g += int(95*a1 + 35*a2)
        b += int(135*a1 + 95*a2)
        row += bytes((min(255,r), min(255,g), min(255,b)))
    rows.append(bytes(row))

# centered geometric Custom OS emblem
for y in range(235, 385):
    for x in range(170, 320):
        dx, dy = x-245, y-310
        if abs(dx)+abs(dy) <= 74:
            i = y*(W*3+1) + 1 + x*3
            rr = bytearray(rows[y])
            if abs(dx)+abs(dy) <= 48:
                rr[x*3+1] = 25
                rr[x*3+2] = 55
            else:
                rr[x*3] = 65
                rr[x*3+1] = 205
                rr[x*3+2] = 255
            rows[y] = bytes(rr)

raw = b"".join(rows)
png = b"\x89PNG\r\n\x1a\n"
png += chunk(b"IHDR", struct.pack(">IIBBBBB", W, H, 8, 2, 0, 0, 0))
png += chunk(b"IDAT", zlib.compress(raw, 9))
png += chunk(b"IEND", b"")
with open(out, "wb") as f:
    f.write(png)
print(out)
