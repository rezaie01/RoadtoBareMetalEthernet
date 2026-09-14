#include <stdlib.h>
#include <string.h>
#include <stdio.h> // TODO: output only if debugging

#include "shared.h"
#include "endian.h"

#include "ethernet_frame.h"


// Etherent Frame header related methonds
const tEthFrameHeader *get_header(const tEthFrame *self);
void set_header(tEthFrame *self, tEthFrameHeader *header);
char *get_header_str(tEthFrameHeader *self);
tEthFrameHeader *create_header(u8 *target_mac, u8 *src_mac, u16 ether_type, u32 vlan_tag);
tEthFrameHeader *parse_header(tEthFrame *self, u8 *bytes, u16 bytes_len);

// Etherent frame related methonds
tEthFrame *parse(tEthFrame *self, u8 *bytes, u16 bytes_len);
char *get_frame_repr_str(tPDU *self);
tPDU *create_frame(tEthFrameHeader *header, u8 *payload, u32 payload_len, tEthFrameFooter *footer);

tEthFrame *tEthFrame_ctor()
{
    tEthFrame *eth_frame = (tEthFrame *)malloc(sizeof(tEthFrame));
    eth_frame->set_header = set_header;
    eth_frame->get_header = get_header;

    eth_frame->create_frame = create_frame;
    eth_frame->create_header = create_header;

    eth_frame->parse_header = parse_header;
    eth_frame->parse = parse;

    return eth_frame;
};

// TODO: set_header(u8* bytes, bytes_len) implement
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
tEthFrameHeader *parse_header(tEthFrame *self, u8 *bytes, u16 bytes_len)
{
    // make sure size is 1B
    if (sizeof(bytes[1]) != 1)
        exit(1); // TODO: improve the exits.

    if (bytes_len >= 14)
    {
        u8 *trgt_mac = bytes;
        u8 *src_mac = bytes + 6;

        u16 ether_type;

        // is an IEEE 802.1Q tagged.
        if (bytes_to_hostu16(bytes[12], bytes[13]) == 0x8100)
        {
            if (bytes_len < 18)
                exit(1);

            u32 vlan_tag = bytes_to_hostu32(bytes[12], bytes[13], bytes[14], bytes[15]);
            ether_type = bytes_to_hostu16(bytes[16], bytes[17]);
            return self->create_header(trgt_mac, src_mac, ether_type, vlan_tag);
        }
        else
        {
            ether_type = bytes_to_hostu16(bytes[12], bytes[13]);
            return self->create_header(trgt_mac, src_mac, ether_type, 0);
        }
    }
    exit(1);
}

tEthFrameHeader *create_header(u8 *target_mac, u8 *src_mac, u16 ether_type, u32 vlan_tag)
{
    tEthFrameHeader *fr_header = (tEthFrameHeader *)malloc(sizeof(tEthFrameHeader));

    fr_header->get_header_str = get_header_str;

    fr_header->target_mac = (u8 *)malloc(sizeof(u8) * 6);
    fr_header->source_mac = (u8 *)malloc(sizeof(u8) * 6);

    memcpy(fr_header->target_mac, target_mac, 6);
    memcpy(fr_header->source_mac, src_mac, 6);

    fr_header->ether_type = ether_type;
    fr_header->vlan_tag = vlan_tag;
    fr_header->len = fr_header->vlan_tag ? 14 + 4 : 14;
    return fr_header;
}

char *get_header_str(tEthFrameHeader *self)
{
    char title[] = "Frame Header:";

    char trgt_mac_title[] = "Target MAC address: ";
    char src_mac_title[] = "Source MAC address: ";
    char mac_format[] = "%s%02x:%02x:%02x:%02x:%02x:%02x";

    char trgt_mac[strlen(trgt_mac_title) + 17 + 1]; // +17 for mac address, +1 for null terminator
    char src_mac[strlen(src_mac_title) + 17 + 1];   // for null terminator

    u8 *tmac = self->target_mac;
    u8 *smac = self->source_mac;
    snprintf(trgt_mac, sizeof(trgt_mac), mac_format, trgt_mac_title, tmac[0], tmac[1], tmac[2], tmac[3], tmac[4], tmac[5]);
    snprintf(src_mac, sizeof(src_mac), mac_format, src_mac_title, smac[0], smac[1], smac[2], smac[3], smac[4], smac[5]);

    char *ether_type_str_format = "0x%04x -- %s";
    u16 size = snprintf(NULL, 0, ether_type_str_format, self->ether_type, ether_type_to_str(self->ether_type));
    char *ether_type_str = (char *)malloc((size + 1) * sizeof(char));
    snprintf(ether_type_str, size + 1, ether_type_str_format, self->ether_type, ether_type_to_str(self->ether_type));

    char *vlan_tag_str = NULL;
    // TODO: Later. Vlan tagged in Detail.
    if (self->vlan_tag)
        vlan_tag_str = "IEEE 802.1Q tagged.";

    char *final_str_format = "%s\n\t%s\n\t%s\n\t%s\n\t%s\n";
    u16 final_size = snprintf(NULL, 0, final_str_format, title, trgt_mac, src_mac, ether_type_str, vlan_tag_str ? vlan_tag_str : "");
    char *final_str = (char *)malloc((final_size + 1) * sizeof(char));
    snprintf(final_str, final_size + 1, final_str_format, title, trgt_mac, src_mac, ether_type_str, vlan_tag_str ? vlan_tag_str : "");

    return final_str;
}

// ========================================  ============================== ==================================
// ========================================  Ethernet Frame Related Methods ==================================
// ========================================  ============================== ==================================

tEthFrame *parse(tEthFrame *self, u8 *bytes, u16 bytes_len)
{

    // TODO: asumming that the bytes is a whole frame, right now.
    // because I don't have access full ethernet packets/frames to test and implement the FCS feature.

    // BUT at least checking if it fullfills the least length requirements.
    // TODO: Watchout for Jumbo or nonstandard frames.
    if (bytes_len < 60 || bytes_len > 1500)
    {
        exit(1);
    }

    tEthFrameHeader *header = self->parse_header(self, bytes, bytes_len);

    tEthFrame *frame_obj = tEthFrame_ctor();
    frame_obj->frame = self->create_frame(header, bytes + header->len, bytes_len - header->len, nullptr);

    return frame_obj;
}

char *get_frame_repr_str(tPDU *self)
{
    tEthFrameHeader *header = (tEthFrameHeader *)self->header;

    char *payload_str = bytes_repr(self->data, self->payload_len, "\t\t");

    char *final_str_format = "Ethernet Frame:\n\t%s\n\tPayload:\n\t\tPayload length: %d\n%s\n";
    u16 final_size = snprintf(NULL, 0, final_str_format, header->get_header_str(header), self->payload_len, payload_str);
    char *final_str = (char *)malloc((final_size + 1) * sizeof(char));
    snprintf(final_str, final_size * 1, final_str_format, header->get_header_str(header), self->payload_len, payload_str);

    return final_str;
}



tPDU *create_frame(tEthFrameHeader *header, u8 *payload, u32 payload_len, tEthFrameFooter *footer)
{
    tPDU *eth_frame = (tPDU *)malloc(sizeof(tPDU));

    eth_frame->get_repr_str = get_frame_repr_str;

    eth_frame->header = header;

    // TODO: asumming that the bytes is a whole frame, right now.
    // because I don't have access full ethernet packets/frames to test and implement the FCS feature.

    // BUT at least checking if it fullfills the least length requirements.
    // TODO: Watchout for Jumbo or nonstandard frames.
    if ((payload_len < 42) || (!header->vlan_tag && payload_len < 46))
    {
        exit(1);
    }

    eth_frame->payload_len = payload_len;
    eth_frame->total_len = header->len + payload_len + (footer ? 4 : 0);
    eth_frame->footer = footer;

    eth_frame->data = (u8 *)malloc(sizeof(u8) * payload_len);
    memcpy(eth_frame->data, payload, payload_len);

    return eth_frame;
}