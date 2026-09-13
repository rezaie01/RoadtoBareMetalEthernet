#ifndef __MNS_ETH_FRM_H__
#define __MNS_ETH_FRM_H__

#include <stdlib.h>
#include <string.h>
#include <stdio.h> // TODO: output only if debugging

#include "pdu.h"
#include "shared.h"

struct tEthFrameHeader;

typedef struct tEthFrameHeader tEthFrameHeader;

typedef struct tEthFrameFooter tEthFrameFooter;

struct tEthFrame;
typedef struct tEthFrame tEthFrame;
struct tEthFrame
{
    tPDU *frame;
    int (*decode)(tEthFrame *self, u8 *bytes);
    u8 *(*encode)(const tEthFrame *self);

    void (*set_header)(tEthFrame *self, tEthFrameHeader *header);
    const tEthFrameHeader *(*get_header)(const tEthFrame *self);

    tEthFrameHeader *(*create_header)(u8 *target_mac, u8 *src_mac, u16 ether_type, u32 vlan_tag);
    tPDU *(*create_frame)(tEthFrameHeader *header, u8 *payload, u32 payload_len, tEthFrameFooter *footer);

    tEthFrameHeader *(*parse_header)(tEthFrame *self, u8 *bytes, u16 bytes_len);
    tEthFrame *(*parse)(tEthFrame *self, u8 *bytes, u16 bytes_len);
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

tEthFrame *tEthFrame_ctor();

#endif