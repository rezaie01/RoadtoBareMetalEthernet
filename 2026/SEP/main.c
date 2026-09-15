#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "endian.h"
#include "shared.h"

#include "ethernet_frame.h"
#include "ippacket_arp.h"

int main()
{

    u8 bytes[] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x0, 0xc, 0x29, 0x11, 0x54, 0xa8, 0x8, 0x6, 0x0, 0x1, 0x8, 0x0, 0x6, 0x4, 0x0, 0x1, 0x0, 0xc, 0x29, 0x11, 0x54, 0xa8, 0xc0, 0xa8, 0x4d, 0x2c, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xc0, 0xa8, 0x4d, 0x9b, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};
    u32 bytes_len = sizeof(bytes) / sizeof(bytes[0]);

    tEthFrame *eth_frame = tEthFrame_ctor();

    tEthFrameHeader *header = eth_frame->parse_header(eth_frame, bytes, bytes_len);
    tPDU *frame = eth_frame->create_frame(header, bytes + header->len, bytes_len - header->len, nullptr);

    // printf(header->get_header_str(header));
    // printf(frame->get_repr_str(frame));
    tEthFrame *parsed_frame = eth_frame->parse(eth_frame, bytes, bytes_len);

    printf(parsed_frame->frame->get_repr_str(parsed_frame->frame));

    u8 *l3_bytes = parsed_frame->frame->data;

    tARPPacket *arp_packet_obj = tARPPacket_ctor();

    tARPPacket* parsed_arp_obj =  arp_packet_obj->parse(arp_packet_obj, l3_bytes, parsed_frame->frame->payload_len);

    printf(parsed_arp_obj->packet->get_repr_str(parsed_arp_obj->packet));

    return EXIT_SUCCESS;
}