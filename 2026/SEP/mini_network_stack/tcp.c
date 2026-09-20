#include <stdio.h>
#include <stdlib.h>

#include "tcp.h"
#include "shared.h"
#include "../endian.h"

tTCPHeader *create_tcp_header(u16 src_port, u16 trgt_port,

                              u32 sequence_num,
                              u32 acknowledge_num,

                              u8 header_length,
                              u8 ctrl_flags,
                              u16 window,

                              u16 checksum,
                              u16 urgent,

                              tTCPHeaderOptions *options);

tTCPHeader *parse_tcp_header(u8 *bytes, u16 bytes_len);

tPDU *create_segment(tTCPHeader *header, u8 *payload, u16 payload_len);
tPDU *parse_segment(u8 *bytes, u16 bytes_len);

char *get_tcp_header_str(tTCPHeader *self);

char *get_tcp_repr_str(tPDU *self);

tTCPProtocol *tTCPProtocol_ctor()
{
    tTCPProtocol *prtcl = malloc(sizeof(tTCPProtocol));

    prtcl->create_header = create_tcp_header;
    prtcl->parse_header = parse_tcp_header;

    prtcl->create_segment = create_segment;
    prtcl->parse_segment = parse_segment;

    return prtcl;
}

tTCPHeader *create_tcp_header(u16 src_port, u16 trgt_port,

                              u32 sequence_num,
                              u32 acknowledge_num,

                              u8 header_length,
                              u8 ctrl_flags,
                              u16 window,

                              u16 checksum,
                              u16 urgent,

                              tTCPHeaderOptions *options)
{

    u8 options_len = options && options->total_len ? options->total_len : 0;
    if (header_length * 4 != 20 + options_len)
    {
        fprintf(stderr, "Missmatch between given header length, and (header fields + header options length)");
        exit(1);
    }

    // TODO: check the checksum
    tTCPHeader *header = malloc(sizeof(tTCPHeader));

    // TODO init the get_header_str
    header->get_header_str = get_tcp_header_str;

    header->src_port = src_port;
    header->trgt_port = trgt_port;

    header->sequence_num = sequence_num;
    header->acknowledge_num = acknowledge_num;

    header->header_length = header_length;
    header->ctrl_flags = ctrl_flags;
    header->window = window;

    header->checksum = checksum;
    header->urgent = urgent;

    header->options = options;

    header->len = 20 + options_len;

    return header;
}

tTCPHeader *parse_tcp_header(u8 *bytes, u16 bytes_len)
{
    if (bytes_len < 20)
    {
        fprintf(stderr, "Invalid length for TCP header bytes");
        exit(1);
    }

    u16 bindex = 0;
    u16 src_port = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u16 trgt_port = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u32 sequence_num = bytes_to_hostu32(bytes[bindex], bytes[bindex + 1], bytes[bindex + 2], bytes[bindex + 3]);

    bindex += 4;
    u32 acknowledge_num = bytes_to_hostu32(bytes[bindex], bytes[bindex + 1], bytes[bindex + 2], bytes[bindex + 3]);

    bindex += 4;
    u8 header_length = (bytes[bindex] & 0xF0) >> 4;

    bindex++;
    u8 ctrl_flags = bytes[bindex];

    bindex++;
    u16 window = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u16 checksum = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u16 urgent = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    tTCPHeaderOptions *options = nullptr;
    if (header_length > 5)
    {
        options = malloc(sizeof(tTCPHeaderOptions));

        // TODO: Rechne options->real_len
        options->total_len = (header_length * 4) - 20;
    }

    return create_tcp_header(
        src_port, trgt_port,
        sequence_num,
        acknowledge_num,
        header_length,
        ctrl_flags,
        window,
        checksum,
        urgent,
        options);
}

char *get_tcp_header_str(tTCPHeader *self)
{

    char *header_length_str = format_str("%d 32-bit blocks (%d bytes)", self->header_length, self->header_length * 4);

    char *ctrl_flags_str = format_str(
        "\n\t\tCWR (Congestion Window Reduced): %s\
        \n\t\tECE  (Explicit Congestion Echo): %s\
        \n\t\tURG                    (Urgent): %s\
        \n\t\tACK            (Acknowledgment): %s\
        \n\t\tPSH                      (Push): %s\
        \n\t\tRST                     (Reset): %s\
        \n\t\tSYN           (Synchronisation): %s\
        \n\t\tFIN                    (Finish): %s\
        ",
        self->ctrl_flags & 0b10000000 ? "set" : "not set",
        self->ctrl_flags & 0b01000000 ? "set" : "not set",
        self->ctrl_flags & 0b00100000 ? "set" : "not set",
        self->ctrl_flags & 0b00010000 ? "set" : "not set",
        self->ctrl_flags & 0b00001000 ? "set" : "not set",
        self->ctrl_flags & 0b00000100 ? "set" : "not set",
        self->ctrl_flags & 0b00000010 ? "set" : "not set",
        self->ctrl_flags & 0b00000001 ? "set" : "not set");

    char *options_str = format_str(self->options && self->options->total_len ? "Header options present." : "No header options.");

    return format_str("TCP Header:\
        \n\tSource port: %d\
        \n\tTarget port: %d\
        \n\tSequence Number: %lu\
        \n\tAcknowledge Number: %lu\
        \n\tHeader length: %s\
        \n\tControl flags: %s\
        \n\tWindow: %d\
        \n\tChecksum: 0x%04x\
        \n\tUrgent: %d\
        \n\tHeader options: %s\
        \n",
                      self->src_port,
                      self->trgt_port,
                      self->sequence_num,
                      self->acknowledge_num,
                      header_length_str,
                      ctrl_flags_str,
                      self->window,
                      self->checksum,
                      self->urgent,
                      options_str);
}

tPDU *create_segment(tTCPHeader *header, u8 *payload, u16 payload_len)
{
    tPDU *segment = malloc(sizeof(tPDU));

    segment->get_repr_str = get_tcp_repr_str;

    segment->header = header;

    segment->data = payload;
    segment->footer = nullptr;
    segment->payload_len = payload_len; // TODO: unreliable payload_len
    segment->total_len = header->len + payload_len;

    return segment;
}

tPDU *parse_segment(u8 *bytes, u16 bytes_len)
{
    if (bytes_len < 20)
    {
        fprintf(stderr, "Unacceptable length of bytes for a TCP Segment");
        exit(1);
    }

    tTCPHeader *header = parse_tcp_header(bytes, bytes_len);

    // TODO: bytes_len, could not be the length of the segment, so better to require
    // that it is the length of segmenet, and in Ethernet also, require that a whole frame is passed for parsing
    tPDU *segment = create_segment(header, bytes + header->len, bytes_len - header->len);

    return segment;
}

char *get_tcp_repr_str(tPDU *self)
{
    tTCPHeader *header = self->header;
    return format_str(
        "TCP Segment:\
        \n\t%s\
        \n\tPayload: (%d Bytes) \
        \n%s\n",
        header->get_header_str(header), self->payload_len, bytes_repr(self->data, self->payload_len, "\t\t"));
}