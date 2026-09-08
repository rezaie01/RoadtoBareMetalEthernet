#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned char u8;
typedef uint16_t u16;
typedef uint32_t u32;

u16 join_to_u16(u8 b1, u8 b0) {
    return (0x0000 | b0) | ((0x0000 | b1) << 8);
}

u32 join_to_u32(u8 b3, u8 b2, u8 b1, u8 b0) {
    return join_to_u16(b1, b0) | (u32)join_to_u16(b3, b2) << 16;
}

int main() {
    u8 bytes[] = {0x45, 0x0, 0x0, 0x3c, 0x1a, 0x53, 0x0, 0x0, 0xff, 0x11, 0x98, 0x2b, 0xc0, 0xa8, 0x48, 0x8a, 0xff, 0xff, 0xff, 0xff, 0x1b, 0xb2, 0x1b, 0xb3, 0x0, 0x28, 0xa3, 0xbe, 0x23, 0x32, 0x30, 0x30, 0x32, 0x38, 0x31, 0x66, 0x38, 0x20, 0x69, 0x70, 0x20, 0x72, 0x65, 0x6a, 0x65, 0x63, 0x74, 0x65, 0x64, 0x20, 0x3a, 0x20, 0x31, 0x34, 0x31, 0x34, 0x31, 0x34, 0x31, 0x34};

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

    if (flags <= 1) { // Fragmentiert: weil die dritte Bit (erste von links) aufs 1 gesetzt ist.
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