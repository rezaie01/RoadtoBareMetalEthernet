
#include <stdlib.h>
#include "types.h"
#include "endian.h"

bool is_host_LE()
{
    // two approaches
    // u16 one = 1;
    // return *((u8 *)&one) == 1; // when pointer casting,

    u8 one_as_BE[2] = {1, 0};
    return *((u16 *)one_as_BE) == 1;
}

u16 u16_to_BE(u16 n)
{
    if (is_host_LE())
    {
        return (n << 8) | ((n & 0xFF00) >> 8);
    }

    return n;
}

u32 u32_to_BE(u32 n)
{
    if (is_host_LE())
    {
        return (n << 24) | ((n & 0x0000FF00) << 8) | ((n & 0x00FF0000) >> 8) | ((n >> 24));
    }

    return n;
}

u8 *u32_to_bytes(u32 n)
{
    u8 *bytes = malloc(4 * sizeof(u8));

    bytes[0] = n >> 24;
    bytes[1] = (n >> 16) & 0xff;
    bytes[2] = (n >> 8) & 0xff;
    bytes[3] = n & 0xff;

    return bytes;
}

// whereas b1 is the more significant byte
u16 bytes_to_hostu16(u8 b1, u8 b0)
{
    return (0x0000 | b0) | ((0x0000 | b1) << 8);
}

// whereas b3 is the most significant byte
u32 bytes_to_hostu32(u8 b3, u8 b2, u8 b1, u8 b0)
{
    return bytes_to_hostu16(b1, b0) | (u32)bytes_to_hostu16(b3, b2) << 16;
}