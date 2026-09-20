#ifndef __MNS_TCP_H__

#define __MNS_TCP_H__

#include "pdu.h"
#include "../types.h"

struct tTCPProtocol;
typedef struct tTCPProtocol tTCPProtocol;

struct tTCPHeader;
typedef struct tTCPHeader tTCPHeader;

struct tTCPHeaderOptions;
typedef struct tTCPHeaderOptions tTCPHeaderOptions;

struct tTCPProtocol
{
    tPDU *segment;

    tTCPHeader *(*create_header)(u16 src_port, u16 trgt_port,

                                     u32 sequence_num,
                                     u32 acknowledge_num,

                                     u8 header_length,
                                     u8 ctrl_flags,
                                     u16 window,

                                     u16 checksum,
                                     u16 urgent,

                                     tTCPHeaderOptions *options);

    tTCPHeader *(*parse_header)(u8 *bytes, u16 bytes_len);

    tPDU *(*create_segment)(tTCPHeader *header, u8 *payload, u16 payload_len);
    tPDU *(*parse_segment)(u8 *bytes, u16 bytes_len);
};

struct tTCPHeader
{
    u16 src_port;
    u16 trgt_port;

    u32 sequence_num;
    u32 acknowledge_num;

    u8 header_length;
    u8 ctrl_flags;
    u16 window;

    u16 checksum;
    u16 urgent;

    tTCPHeaderOptions *options;

    u16 len;

    char *(*get_header_str)(tTCPHeader *self);
};

struct tTCPHeaderOptions
{
    u8 real_len;
    u8 total_len;
};

tTCPProtocol *tTCPProtocol_ctor();

#endif