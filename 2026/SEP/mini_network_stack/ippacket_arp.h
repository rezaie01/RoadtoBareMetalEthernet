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

    void (*set_header)(tARPPacket *self, tIPARPHeader *header);
    const tIPARPHeader *(*get_header)(const tARPPacket *self);

    tIPARPHeader *(*create_header)(
        u16 hardware_type, u16 prtcl_type,
        u8 hw_size, u8 prtcl_size,

        u16 operation,
        u8 *source_mac, u8 *source_addr,

        u8 *target_mac, u8 *target_addr);

    tIPARPHeader *(*parse_header)(tARPPacket *self, u8 *bytes, u16 bytes_len);

    tPDU *(*create_packet)(tIPARPHeader *header);
    tARPPacket *(*parse)(tARPPacket *self, u8 *bytes, u16 bytes_len);
};

typedef enum OpCode
{
    REQUEST = 1,
    REPLY = 2
} OpCode;

tARPPacket *tARPPacket_ctor();

#endif