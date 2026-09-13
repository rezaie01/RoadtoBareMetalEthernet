#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "shared.h"

char *bytes_repr(u8 *bytes, u32 size, char* line_prefix)
{
    size_t prefix_len = strlen(line_prefix);
    size_t str_size = (size * 3) + ((size/16 + 1) * prefix_len) + 1; // every bytes will take a length a 3 (2 hex digits, a trailing space or a new line character) plus the null terminator

    char *str = malloc(str_size);
    for (u32 i = 0; i <= size/16; i++)
    {
        strcat(str, line_prefix);
        for (u8 j = 0; j < 16 && (i * 16) + j < size; j++) {

            char src[4];
            if (j == 15 || (i * 16) + j == size - 1) {
                snprintf(src, 3, "%02x", bytes[i * 16 + j]);
            } else {
                snprintf(src, 4, "%02x ", bytes[i * 16 + j]);
            }
            strcat(str, src);
        }
        strcat(str, "\n");

    }

    return str;
}


char *ether_type_to_str(u16 eth_type)
{
    switch (eth_type)
    {
    case 0x0800:
        return "IPv4";
    case 0x86dd:
        return "IPv6";
    case 0x0806:
        return "ARP (Address Resolution Protocol)";
    case 0x0842:
        return "WoL (Wake on LAN)";
    default: // TODO: Later. more EtherTypes.
        return "Unsupported EtherType";
    }
}
