#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

typedef uint32_t u32;
typedef uint16_t u16;

/* 2 Bytes big endian array to u16 */
u16 read_u16_be(u32 bytes[2]);
u32 read_u32_be(u32 bytes[4]);
u32 *write_u16_be(u16 u16_num);
u32 *write_u32_be(u32 u32_num);

int main()
{
    u32 u16_be[2];
    scanf("%02x %02x", &u16_be[0], &u16_be[1]);
    printf("%04x\n", read_u16_be(u16_be));

    u32 *output_u16_be = write_u16_be(read_u16_be(u16_be));
    printf("%02x %02x\n", output_u16_be[0], output_u16_be[1]);

    u32 u32_be[4];
    scanf("%02x %02x %02x %02x", &u32_be[0], &u32_be[1], &u32_be[2], &u32_be[3]);
    printf("%08x\n", read_u32_be(u32_be));

    u32 *output_u32_be = write_u32_be(read_u32_be(u32_be));
    printf("%02x %02x %02x %02x\n", output_u32_be[0], output_u32_be[1], output_u32_be[2], output_u32_be[3]);

    return EXIT_SUCCESS;
}

u16 read_u16_be(u32 bytes[2])
{
    return (u16)bytes[0] * 0x0100 + (u16)bytes[1];
}

u32 read_u32_be(u32 bytes[4])
{
    u16 half_1st = (u16)bytes[2] * 0x0100 + (u16)bytes[3];
    u16 half_2nd = (u16)bytes[0] * 0x0100 + (u16)bytes[1];
    return (u32)half_2nd * 0x010000 + (u32)half_1st;
}

u32 *write_u16_be(u16 u16_num)
{
    u32 *u16_be = (u32 *)malloc(2 * sizeof(u32));
    u16_be[0] = u16_num / 0x0100;
    u16_be[1] = u16_num % 0x0100;
    return u16_be;
}

u32 *write_u32_be(u32 u32_num)
{
    u32 *u32_be = (u32 *)malloc(4 * sizeof(u32));

    u32 u32_remainder = u32_num;
    for (size_t i = 0; i < 4; i++)
    {
        u32 exp = pow(0x10, (6 - (i * 2)));
        u32_be[i] = u32_remainder / exp;

        u32_remainder %= exp;
    }

    return u32_be;
}
