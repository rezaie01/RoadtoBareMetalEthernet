#ifndef __MNS_ETH_FRM_H__
#define __MNS_ETH_FRM_H__

#include <stdlib.h>
#include <stdio.h> // TODO: output only if debugging

#include "pdu.h"

typedef struct tEthFrameHeader
{
    u8 *reciever_mac;
    u8 *sender_mac;
    u16 ether_type;
    u32 vlan_tag;
    u8 len;
} tEthFrameHeader;

typedef struct tEthFrameFooter
{
    u8 *checksum;
} tEthFrameFooter;

struct tEthFrame;

typedef struct tEthFrame tEthFrame;
struct tEthFrame
{
    tPDU *frame;
    int (*decode)(tEthFrame *self, u8 *bytes);
    u8 *(*encode)(const tEthFrame *self);

    void (*set_header)(tEthFrame *self, tEthFrameHeader *header);
    const tEthFrameHeader *(*get_header)(const tEthFrame *self);

    tEthFrameHeader* (*create_header)(u8 *target_mac, u8 *src_mac, u16 ether_type, u32 vlan_tag);
tEthFrameHeader* (*parse_header)(tEthFrame *self, u8 *bytes, u16 bytes_len);
};

tEthFrame *tEthFrame_ctor();

void set_header(tEthFrame *self, tEthFrameHeader *header)
{
    if (!self->frame)
    {
        self->frame = (tPDU *)malloc(sizeof(tPDU));
    }

    self->frame->header = header;
}

const tEthFrameHeader *get_header(const tEthFrame *self)
{
    const tEthFrameHeader *header = (tEthFrameHeader *)self->frame->header;
    return header;
}

// array of header bytes or the whole frame bytes.
tEthFrameHeader *parse_header(tEthFrame *self, u8 *bytes, u16 bytes_len) {
    // make sure size is 1B
    if (sizeof(bytes[1]) != 1) exit(1); // TODO: improve the exits.

    if (bytes_len >= 14) {
        u8* trgt_mac = bytes;
        u8* src_mac = bytes + 6;
        
        u16 ether_type;

        // is an IEEE 802.1Q tagged.
        if (bytes_to_hostu16(bytes[12], bytes[13]) == 0x8100) {
            if (bytes_len < 18) exit(1);

            u32 vlan_tag = bytes_to_hostu32(bytes[12], bytes[13], bytes[14], bytes[15]);
            ether_type = bytes_to_hostu16(bytes[16], bytes[17]);
            return self->create_header(trgt_mac, src_mac, ether_type, vlan_tag);
        } else {
            ether_type = bytes_to_hostu16(bytes[16], bytes[17]);
            return self->create_header(trgt_mac, src_mac, ether_type, 0);
        }
    }
    exit(1);

}

tEthFrameHeader *create_header(u8 *target_mac, u8 *src_mac, u16 ether_type, u32 vlan_tag)
{
    tEthFrameHeader *fr_header = (tEthFrameHeader *)malloc(sizeof(tEthFrameHeader));
    fr_header->reciever_mac = (u8 *)malloc(sizeof(u8) * 6);
    fr_header->sender_mac = (u8 *)malloc(sizeof(u8) * 6);

    memcpy(fr_header->reciever_mac, target_mac, 6);
    memcpy(fr_header->sender_mac, src_mac, 6);

    fr_header->ether_type = ether_type;
    fr_header->vlan_tag = vlan_tag;
    fr_header->len = fr_header->vlan_tag ? 14 + 4 : 14;
    return fr_header;
}

tEthFrame *tEthFrame_ctor()
{
    tEthFrame *eth_frame = (tEthFrame *)malloc(sizeof(tEthFrame));
    eth_frame->set_header = set_header;
    eth_frame->get_header = get_header;
    eth_frame->create_header = create_header;
    eth_frame->parse_header = parse_header;

    return eth_frame;
};
#endif