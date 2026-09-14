#ifndef __MNS_IPPACKET_ARP_H__
#define __MNS_IPPACKET_ARP_H__

#include "types.h"
#include "pdu.h"

struct tIPARPHeader;

typedef struct tIPARPHeader tIPARPHeader;

struct tIPARPHeader
{
    u16 hardware_type;
    u16 prtcl_type;

    u8 hw_size;
    u8 prtcl_size;

    u16 operation;
    u8 *source_mac;
    u8 *source_addr;

    u8 *target_mac;
    u8 *target_addr;

    u8 len;

    char *(*get_arp_header_str)(tIPARPHeader *self);
};

struct tARPPacket;

typedef struct tARPPacket tARPPacket;
struct tARPPacket
{
    tPDU *packet;
    int (*decode)(tARPPacket *self, u8 *bytes, u16 bytes_len);
    u8 *(*encode)(const tARPPacket *self);

    void (*set_arp_header)(tARPPacket *self, tIPARPHeader *header);
    const tIPARPHeader *(*get_arp_header)(const tARPPacket *self);

    tIPARPHeader* (*create_arp_header)(
        u16 hardware_type, u16 prtcl_type,
        u8 hw_size, u8 prtcl_size,

        u16 operation,
        u8 *source_mac, u8 *source_addr,

        u8 *target_mac, u8 *target_addr);

    tIPARPHeader* (*parse_arp_header)(tARPPacket *self, u8 *bytes, u16 bytes_len);

    // tPDU *(*create_packet)(tIPARPHeader *header, u8 *payload, u32 payload_len);
    // tARPPacket *(*parse)(tARPPacket *self, u8 *bytes, u16 bytes_len);
};

typedef enum OpCode
{
    REQUEST = 1,
    REPLY = 2
} OpCode;

tARPPacket *tARPPacket_ctor();

// tARPPacket *parse(tARPPacket *self, u8 *bytes, u16 bytes_len)
// {

//     // TODO: asumming that the bytes is a whole frame, right now.
//     // because I don't have access full ethernet packets/frames to test and implement the FCS feature.

//     // BUT at least checking if it fullfills the least length requirements.
//     // TODO: Watchout for Jumbo or nonstandard frames.
//     if (bytes_len < 60 || bytes_len > 1500)
//     {
//         exit(1);
//     }

//     tIPARPHeader *header = self->parse_header(self, bytes, bytes_len);

//     tARPPacket *frame_obj = tARPPacket_ctor();
//     frame_obj->packet = self->create_packet(header, bytes + header->len, bytes_len - header->len, nullptr);

//     return frame_obj;
// }

// // array of header bytes or the whole frame bytes.
// tIPARPHeader *parse_header(tARPPacket *self, u8 *bytes, u16 bytes_len)
// {
//     // make sure size is 1B
//     if (sizeof(bytes[1]) != 1)
//         exit(1); // TODO: improve the exits.

//     if (bytes_len >= 14)
//     {
//         u8 *trgt_mac = bytes;
//         u8 *src_mac = bytes + 6;

//         u16 ether_type;

//         // is an IEEE 802.1Q tagged.
//         if (bytes_to_hostu16(bytes[12], bytes[13]) == 0x8100)
//         {
//             if (bytes_len < 18)
//                 exit(1);

//             u32 vlan_tag = bytes_to_hostu32(bytes[12], bytes[13], bytes[14], bytes[15]);
//             ether_type = bytes_to_hostu16(bytes[16], bytes[17]);
//             return self->create_arp_header(trgt_mac, src_mac, ether_type, vlan_tag);
//         }
//         else
//         {
//             ether_type = bytes_to_hostu16(bytes[12], bytes[13]);
//             return self->create_arp_header(trgt_mac, src_mac, ether_type, 0);
//         }
//     }
//     exit(1);
// }

// char *bytes_repr(u8 *bytes, u32 size, char *line_prefix)
// {
//     size_t prefix_len = strlen(line_prefix);
//     size_t str_size = (size * 3) + ((size / 16 + 1) * prefix_len) + 1; // every bytes will take a length a 3 (2 hex digits, a trailing space or a new line character) plus the null terminator

//     char *str = malloc(str_size);
//     for (u32 i = 0; i <= size / 16; i++)
//     {
//         strcat(str, line_prefix);
//         for (u8 j = 0; j < 16 && (i * 16) + j < size; j++)
//         {

//             char src[4];
//             if (j == 15 || (i * 16) + j == size - 1)
//             {
//                 snprintf(src, 3, "%02x", bytes[i * 16 + j]);
//             }
//             else
//             {
//                 snprintf(src, 4, "%02x ", bytes[i * 16 + j]);
//             }
//             strcat(str, src);
//         }
//         strcat(str, "\n");
//     }

//     return str;
// }

// char *get_frame_repr_str(tPDU *self)
// {
//     tIPARPHeader *header = (tIPARPHeader *)self->header;

//     char *payload_str = bytes_repr(self->data, self->payload_len, "\t\t");

//     char *final_str_format = "Ethernet Frame:\n\t%s\n\tPayload:\n\t\tPayload length: %d\n%s\n";
//     u16 final_size = snprintf(NULL, 0, final_str_format, header->get_arp_header_str(header), self->payload_len, payload_str);
//     char *final_str = (char *)malloc((final_size + 1) * sizeof(char));
//     snprintf(final_str, final_size * 1, final_str_format, header->get_arp_header_str(header), self->payload_len, payload_str);

//     return final_str;
// }

// tPDU *create_frame(tIPARPHeader *header, u8 *payload, u32 payload_len, tEthFrameFooter *footer)
// {
//     tPDU *eth_frame = (tPDU *)malloc(sizeof(tPDU));

//     eth_frame->get_repr_str = get_frame_repr_str;

//     eth_frame->header = header;

//     // TODO: asumming that the bytes is a whole frame, right now.
//     // because I don't have access full ethernet packets/frames to test and implement the FCS feature.

//     // BUT at least checking if it fullfills the least length requirements.
//     // TODO: Watchout for Jumbo or nonstandard frames.
//     if ((payload_len < 42) || (!header->vlan_tag && payload_len < 46))
//     {
//         exit(1);
//     }

//     eth_frame->payload_len = payload_len;
//     eth_frame->total_len = header->len + payload_len + footer ? 4 : 0;
//     eth_frame->footer = footer;

//     eth_frame->data = (u8 *)malloc(sizeof(u8) * payload_len);
//     memcpy(eth_frame->data, payload, payload_len);

//     return eth_frame;
// }

#endif