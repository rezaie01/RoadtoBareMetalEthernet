#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

/* 2 Bytes big endian array to u16 */
u16 read_u16_be(u32 bytes[2]);
u32 read_u32_be(u32 bytes[4]);
void write_u16_be(u16 u16_num, u32 *bytes);
void write_u32_be(u32 u32_num, u32 *bytes);

int main()
{
    u32 u16_be[2];
    scanf("%02x %02x", &u16_be[0], &u16_be[1]);
    printf("%04x\n", read_u16_be(u16_be));

    u32 output_u16_be[2] = {0};
    write_u16_be(read_u16_be(u16_be), output_u16_be);
    printf("%02x %02x\n", output_u16_be[0], output_u16_be[1]);

    u32 u32_be[4];
    scanf("%02x %02x %02x %02x", &u32_be[0], &u32_be[1], &u32_be[2], &u32_be[3]);
    printf("%08x\n", read_u32_be(u32_be));

    u32 output_u32_be[4] = {0};
    write_u32_be(read_u32_be(u32_be), output_u32_be);
    printf("%02x %02x %02x %02x\n", output_u32_be[0], output_u32_be[1], output_u32_be[2], output_u32_be[3]);

    return EXIT_SUCCESS;
}

u16 read_u16_be(u32 bytes[2])
{
    u32 tmp = bytes[0] << 8;
    return tmp | bytes[1];
}

u32 read_u32_be(u32 bytes[4])
{
    u32 tmp = bytes[0] << 24;
    tmp |= bytes[1] << 16;
    tmp |= bytes[2] << 8;
    tmp |= bytes[3];
    return tmp;
}

void write_u16_be(u16 u16_num, u32 *bytes)
{
    bytes[0] = u16_num >> 8;
    bytes[1] = u16_num & 0x00ff;
}

void write_u32_be(u32 u32_num, u32 *bytes)
{
    bytes[0] = u32_num >> 24;
    bytes[1] = (u32_num & 0x00ff0000) >> 16;
    bytes[2] = (u32_num & 0x0000ff00) >> 8;
    bytes[3] = u32_num & 0x000000ff;
}
