#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "shared.h"

char *bytes_repr(u8 *bytes, u32 size, char *line_prefix)
{
    size_t prefix_len = line_prefix ? strlen(line_prefix) : 0;
    size_t str_size = (size * 3) + ((size / 16 + 1) * prefix_len) + 1; // every bytes will take a length a 3 (2 hex digits, a trailing space or a new line character) plus the null terminator

    char *str = malloc(str_size);
    for (u32 i = 0; i <= size / 16; i++)
    {
        strcat(str, line_prefix ? line_prefix : "");
        for (u8 j = 0; j < 16 && (i * 16) + j < size; j++)
        {

            char src[4];
            if (j == 15 || (i * 16) + j == size - 1)
            {
                snprintf(src, 3, "%02x", bytes[i * 16 + j]);
            }
            else
            {
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

char *bytes_to_mac_str(u8 *mac, char *prefix)
{

    char *mac_str = nullptr;
    if (!prefix)
    {
        char mac_format[] = "%02x:%02x:%02x:%02x:%02x:%02x";
        u16 size = snprintf(NULL, 0, mac_format, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        mac_str = (char *)malloc((size + 1) * sizeof(char));
        snprintf(mac_str, size + 1, mac_format, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return mac_str;
    }
    else
    {

        char mac_format[] = "%s%02x:%02x:%02x:%02x:%02x:%02x";
        u16 size = snprintf(NULL, 0, mac_format, prefix, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        mac_str = (char *)malloc((strlen(prefix) + 17 + 1) * sizeof(char));
        snprintf(mac_str, size + 1, mac_format, prefix, mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
        return mac_str;
    }
}

char *bytes_to_ipv4_address_str(u8 *addr, char *prefix)
{
    if (!prefix)
    {
        char addr_format[] = "%d.%d.%d.%d";
        u16 size = snprintf(NULL, 0, addr_format, addr[0], addr[1], addr[2], addr[3]);
        char *addr_str = (char *)malloc((size + 1) * sizeof(char));
        snprintf(addr_str, size + 1, addr_format, addr[0], addr[1], addr[2], addr[3]);
        return addr_str;
    }
    else
    {
        char addr_format[] = "%s%d.%d.%d.%d";
        u16 size = snprintf(NULL, 0, addr_format, prefix, addr[0], addr[1], addr[2], addr[3]);
        char *addr_str = (char *)malloc((size + 1) * sizeof(char));
        snprintf(addr_str, size + 1, addr_format, prefix, addr[0], addr[1], addr[2], addr[3]);
        return addr_str;
    }
}