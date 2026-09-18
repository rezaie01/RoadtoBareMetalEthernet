#ifndef __MNS_ETH_FRM_H__
#define __MNS_ETH_FRM_H__

#include "types.h"
#include "pdu.h"

struct tEthFrameHeader;

typedef struct tEthFrameHeader tEthFrameHeader;

typedef struct tEthFrameFooter tEthFrameFooter;

struct tEthProtocol;
typedef struct tEthProtocol tEthProtocol;
struct tEthProtocol
{
    tPDU *frame;
    int (*decode)(tEthProtocol *self, u8 *bytes);
    u8 *(*encode)(const tEthProtocol *self);

    void (*set_header)(tEthProtocol *self, tEthFrameHeader *header);
    const tEthFrameHeader *(*get_header)(const tEthProtocol *self);

    tEthFrameHeader *(*create_header)(u8 *target_mac, u8 *src_mac, u16 ether_type, u32 vlan_tag);
    tPDU *(*create_frame)(tEthFrameHeader *header, u8 *payload, u32 payload_len, tEthFrameFooter *footer);

    tEthFrameHeader *(*parse_header)(tEthProtocol *self, u8 *bytes, u16 bytes_len);
    tEthProtocol *(*parse)(tEthProtocol *self, u8 *bytes, u16 bytes_len);
};

struct tEthFrameHeader
{
    u8 *target_mac;
    u8 *source_mac;
    u16 ether_type;
    u32 vlan_tag;
    u8 len;

    char *(*get_header_str)(tEthFrameHeader *self);
};

struct tEthFrameFooter
{
    u8 *checksum;
};

tEthProtocol *tEthFrame_ctor();

#endif