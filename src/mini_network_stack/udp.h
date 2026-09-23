#ifndef __MNS_UDP_H__
#define __MNS_UDP_H__

#include "pdu.h"
#include "../types.h"

struct tUDPProtocol;
typedef struct tUDPProtocol tUDPProtocol;

struct tUDPHeader;
typedef struct tUDPHeader tUDPHeader;

struct tUDPProtocol
{
    tPDU *datagram;

    tUDPHeader *(*create_header)(u16 src_port, u16 trgt_port, u16 total_length, u16 checksum);
    tUDPHeader *(*parse_header)(tUDPProtocol *self, u8 *bytes, u16 bytes_len);

    tPDU *(*create_datagram)(tUDPHeader *header, u8 *payload, u16 payload_len);
    tPDU *(*parse_datagram)(tUDPProtocol *self, u8 *bytes, u16 bytes_len);
};

struct tUDPHeader
{
    u16 src_port;
    u16 trgt_port;

    u16 total_length;
    u16 checksum;

    u16 len;

    char *(*get_header_str)(tUDPHeader *self);
};

tUDPProtocol *tUDPProtocol_ctor();

#endif