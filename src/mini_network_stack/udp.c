
#include <stdio.h>
#include <stdlib.h>

#include "udp.h"
#include "shared.h"
#include "../endian.h"

tUDPHeader *create_udp_header(u16 src_port, u16 trgt_port, u16 total_length, u16 checksum);
tUDPHeader *parse_udp_header(tUDPProtocol *self, u8 *bytes, u16 bytes_len);
char *get_udp_header_str(tUDPHeader *self);

tPDU *create_datagram(tUDPHeader *header, u8 *payload, u16 payload_len);
tPDU *parse_datagram(tUDPProtocol *self, u8 *bytes, u16 bytes_len);
char *get_udp_repr_str(tPDU *self);

tUDPProtocol *tUDPProtocol_ctor()
{
    tUDPProtocol *prtcl = malloc(sizeof(tUDPProtocol));

    prtcl->create_header = create_udp_header;
    prtcl->parse_header = parse_udp_header;

    prtcl->create_datagram = create_datagram;
    prtcl->parse_datagram = parse_datagram;

    return prtcl;
}

tUDPHeader *create_udp_header(u16 src_port, u16 trgt_port, u16 total_length, u16 checksum)
{

    u16 max_dg_length = (1024 * 16 - 1) - 8; // -8 weil UDP Header 8 Bytes lang ist.
    if (total_length > max_dg_length)
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
    header->total_length = total_length;
    header->checksum = checksum;

    header->len = 8;

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
    u16 total_length = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u16 checksum = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    return self->create_header(src_port, trgt_port, total_length, checksum);
}

char *get_udp_header_str(tUDPHeader *self)
{
    return format_str("UDP Header:\
        \n\tSource port: %d\
        \n\tTarget port: %d\
        \n\tDatagram length: %d\
        \n\tChecksum: 0x%04x\
        \n",
                      self->src_port,
                      self->trgt_port,
                      self->total_length,
                      self->checksum);
}

tPDU *create_datagram(tUDPHeader *header, u8 *payload, u16 payload_len)
{

    if (header->total_length - 8 > payload_len)
    {
        fprintf(stderr, "Missmatch between length of Input bytes, and the length specified in header.");
        exit(1);
    }

    tPDU *datagram = malloc(sizeof(tPDU));

    datagram->get_repr_str = get_udp_repr_str;

    datagram->header = header;
    datagram->data = payload;
    datagram->footer = nullptr;
    datagram->payload_len = header->total_length - header->len;
    datagram->total_len = header->total_length;

    return datagram;
}

tPDU *parse_datagram(tUDPProtocol* self, u8* bytes, u16 bytes_len) {
    if (bytes_len < 8) {
        fprintf(stderr, "Unacceptable length of bytes for a UDP datagram");
        exit(1);
    }

    tUDPHeader *header = self->parse_header(self, bytes, bytes_len);

    if (bytes_len < header->total_length) {
        fprintf(stderr, "Missmatch between length specified in header and the number of input bytes");
        exit(1);
    }
    
    tPDU *datagram = create_datagram(header, bytes+header->len, header->total_length - header->len);

    return datagram;
}

char *get_udp_repr_str(tPDU *self)
{
    tUDPHeader *header = self->header;
    return format_str(
        "UDP Datagram:\
        \n\t%s\
        \n\tPayload: (%d Bytes) \
        \n%s\n",
        header->get_header_str(header), self->payload_len, bytes_repr(self->data, self->payload_len, "\t\t"));
}