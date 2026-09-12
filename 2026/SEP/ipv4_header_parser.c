#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "types.h"


u16 join_to_u16(u8 b1, u8 b0) {
    return (0x0000 | b0) | ((0x0000 | b1) << 8);
}

u32 join_to_u32(u8 b3, u8 b2, u8 b1, u8 b0) {
    return join_to_u16(b1, b0) | (u32)join_to_u16(b3, b2) << 16;
}

int parse_ipv4_header(u8 bytes[]) {
    u8 version = bytes[0] >> 4;
    u8 i_h_l = bytes[0] & 0b00001111;

    u32 header_len = i_h_l * 4; 

    printf("Version: %d\n", version);
    printf("Length: %d => %d * 32 = %d Bits\n", i_h_l, i_h_l, i_h_l * 32);

    // Das zweite Byte is ToS, was nicht analysiert wird.
    
    u16 total_len = join_to_u16(bytes[2], bytes[3]);
    printf("Total Length: %d Bytes\n", total_len);


    u16 id = join_to_u16(bytes[4], bytes[5]);
    printf("Identification: 0x%x\n", id);

    u8 flags = (bytes[6] & 0b11100000) >> 5;

    if (flags != 2) { // Fragmentiert: weil die dritte Bit (erste von links) aufs 1 gesetzt ist.
        printf("Flags: %03d\n", flags);
        printf(flags == 0  ? "\t Flag is set to 0 => Fragmentation happend, and this packet is the last fragment." : "\tFlag is set to 1 => Fragmentation happend.");
        puts("");
    } else {
        printf("Flags: %d => Fragmentation is not allowed.\n", flags);
    }

    u16 frag_offset = join_to_u16(bytes[6], bytes[7]) & 0x1fff;
    printf("Fragmentation offset: %d \n", frag_offset);

    if (frag_offset + (flags<<13) != join_to_u16(bytes[6], bytes[7])) return EXIT_FAILURE;

    u8 ttl = bytes[8];
    printf("Time-to-Live: %d (number of Hops [routers])\n", ttl);

    u8 protocol = bytes[9];
    printf("Protocol: %d ", protocol);

    switch(protocol) {
        case 6: printf("(TCP)\n"); break;
        case 17: printf("(UDP)\n"); break;
        case 1: printf("(ICMP)\n"); break;
        case 2: printf("(IGMP)\n"); break;
        case 58: printf("(IPv6-ICMP)\n"); break;
        default: puts("Unkown Protocol. Known protocols are: TCP, UDP, ICMP, IPv6-ICMP, and IGMP\n");
    }

    // 11th & 12th Bytes represent the Header-Checksum, and is not currently supported for Analysis.

    printf("Source Address: %d.%d.%d.%d\n", bytes[12], bytes[13], bytes[14], bytes[15])  ;
    printf("Destination Address: %d.%d.%d.%d\n", bytes[16], bytes[17], bytes[18], bytes[19])  ;
   
    return EXIT_SUCCESS;
}
