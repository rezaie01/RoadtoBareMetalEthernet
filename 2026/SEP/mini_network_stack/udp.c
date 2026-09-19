
#include <stdio.h>

#include "udp.h"
#include "shared.h"
#include "../endian.h"

tUDPHeader *create_udp_header(u16 src_port, u16 trgt_port, u16 length, u16 checksum);
tUDPHeader *parse_udp_header(tUDPProtocol *self, u8 *bytes, u16 bytes_len);
char *get_udp_header_str(tUDPHeader *self);

tUDPHeader *create_udp_header(u16 src_port, u16 trgt_port, u16 len, u16 checksum)
{

    if (len > 65507)
    {
        fprintf(stderr, "Invalid Datagram length");
        exit(1);
    }
    // TODO: check the checksum
    tUDPHeader *header = malloc(sizeof(tUDPHeader));

    // TODO init the get_header_str
    header->get_header_str = get_udp_header_str;

    header->src_port = src_port;
    header->trgt_port = trgt_port;
    header->len = len;
    header->checksum = checksum;

    return header;
}

tUDPHeader *parse_udp_header(tUDPProtocol *self, u8 *bytes, u16 bytes_len)
{
    if (bytes_len < 8)
    {
        fprintf(stderr, "Invalid length for UDP header bytes");
        exit(1);
    }

    u16 bindex = 0;
    u16 src_port = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u16 trgt_port = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u16 len = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u16 checksum = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    return self->create_header(src_port, trgt_port, len, checksum);
}

char *get_udp_header_str(tUDPHeader *self)
{
    return format_str("UDP Header:\
        \n\tSource port: %d\
        \n\tTarget port: %d\
        \n\tDatagram length: %d\
        \n\tDatagram length: 0x%04x\
        \n",
                      self->src_port,
                      self->trgt_port,
                      self->len,
                      self->checksum);
}
