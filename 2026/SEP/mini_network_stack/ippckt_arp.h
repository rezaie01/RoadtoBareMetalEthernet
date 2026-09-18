#ifndef __MNS_IPPCKT_ARP_H__
#define __MNS_IPPCKT_ARP_H__

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

    char *(*get_header_str)(tIPARPHeader *self);
};

struct tARPProtocol;

typedef struct tARPProtocol tARPProtocol;
struct tARPProtocol
{
    tPDU *packet;
    int (*decode)(tARPProtocol *self, u8 *bytes, u16 bytes_len);
    u8 *(*encode)(const tARPProtocol *self);

    void (*set_header)(tARPProtocol *self, tIPARPHeader *header);
    const tIPARPHeader *(*get_header)(const tARPProtocol *self);

    tIPARPHeader *(*create_header)(
        u16 hardware_type, u16 prtcl_type,
        u8 hw_size, u8 prtcl_size,

        u16 operation,
        u8 *source_mac, u8 *source_addr,

        u8 *target_mac, u8 *target_addr);

    tIPARPHeader *(*parse_header)(tARPProtocol *self, u8 *bytes, u16 bytes_len);

    tPDU *(*create_packet)(tIPARPHeader *header);
    tARPProtocol *(*parse)(tARPProtocol *self, u8 *bytes, u16 bytes_len);
};

typedef enum OpCode
{
    REQUEST = 1,
    REPLY = 2
} OpCode;

tARPProtocol *tARPProtocol_ctor();

#endif