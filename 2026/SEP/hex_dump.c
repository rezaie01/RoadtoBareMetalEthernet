#include <stdio.h>
#include <ctype.h>

#include "types.h"
#include "hex_dump.h"

void hex_dump(const u8 *bytes, size_t size)
{
    size_t i = 0;
    for (size_t row = 0; row <= (size / 16) + (size % 16 > 0 ? 1 : 0); row++)
    {
        printf("%08x", (row * 16 > size ? size : row * 16));

        size_t j = i;
        for (; j < (row + 1) * 16 && j < size; j++)
        {
            printf(" %02x", (u8)bytes[j]);
        }

        if (size > 0 && row * 16 < size) // only if the size is non-zero, and this is not the empty row.
        {
            if ((row + 1) * 16 > size) // next row is the empty row.
            {
                for (int k = 0; k < 16 - (j % 16); k++)
                    printf("   ");
            }
            printf(" |");

            for (j = i; j < (row + 1) * 16 && j < size; j++)
            {
                char c = bytes[j];
                printf("%c", isgraph(c)? c:'.');
            }

            printf("|");
        }

        printf("\n");
        i = j;
    }
}