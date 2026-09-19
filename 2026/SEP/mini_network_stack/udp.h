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

    tUDPHeader *(*create_header)(u16 src_port, u16 trgt_port, u16 len, u16 checksum);
    tUDPHeader *(*parse_udp_header)(tUDPProtocol *self, u8 *bytes, u16 bytes_len);
};

struct tUDPHeader
{
    u16 src_port;
    u16 trgt_port;

    u16 len;
    u16 checksum;


    char *(*get_header_str)(tUDPHeader *self);
};

tPDU *tUDPProtocol_ctor();

#endif