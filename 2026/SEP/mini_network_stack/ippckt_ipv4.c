#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ippckt_ipv4.h"
#include "../endian.h"
#include "shared.h"

char *get_ipv4_header_str(tIPv4Header *self);
tIPv4Header *create_ipv4_header(
    IP_VERSION version, u8 IHL, u8 TOS, u16 total_len,
    u16 id, u8 flags, u16 frag_offset,
    u8 TTL, IP_PRTCL_NUM prtcl, u16 checksum,
    u8 *src_ip,
    u8 *trgt_ip,

    tIPv4HeaderOptions *options);

tIPv4Header *parse_ipv4_header(tIPv4Protocol *self, u8 *bytes, u16 bytes_len);

tPDU *create_ipv4_packet(
    tIPv4Header *header,
    u8 *payload,
    u16 payload_len);

tPDU *parse_ipv4_packet(
    tIPv4Protocol *self,
    u8 *bytes,
    u16 bytes_len);

char *get_ipv4_repr_str(tPDU *self);

//
//
//
//
//
//

tIPv4Protocol *tIPv4_ctor()
{
    tIPv4Protocol *ipv4_prtcl = (tIPv4Protocol *)malloc(sizeof(tIPv4Protocol));

    ipv4_prtcl->create_header = create_ipv4_header;
    ipv4_prtcl->parse_header = parse_ipv4_header;

    ipv4_prtcl->create_packet = create_ipv4_packet;
    ipv4_prtcl->parse_packet = parse_ipv4_packet;

    return ipv4_prtcl;
}

tIPv4Header *create_ipv4_header(
    IP_VERSION version, u8 IHL, u8 TOS, u16 total_len,
    u16 id, u8 flags, u16 frag_offset,
    u8 TTL, IP_PRTCL_NUM prtcl, u16 checksum,
    u8 *src_ip,
    u8 *trgt_ip,

    tIPv4HeaderOptions *options)
{
    if (version != VR_4)
    {
        fprintf(stderr, "Called create_ipv4_header with a non-4 IP version.\n");
        exit(1);
    }

    if (IHL < 5 || IHL > 12 || (options && options->len > 40) || total_len < 20 || total_len > (64 * 1024 - 1)) // max header length = 60.  Options max length = 40
    // MAX total_len = (64 - 1)KB, dass wir es hier nicht prüfen können. Aber falls Overflow ist passiert => wird es umgerechnet => total < 20.
    {
        fprintf(stderr, "Invalid Internet header length (IHL) OR Invalid length options OR Invalid Total length: IHL: %d, Options length: %d, Total length: %d\n", IHL, options->len, total_len);
        exit(1);
    }

    // TODO: arbeite an TOS später

    // stelle sicher, dass nur die zwei erste bits Kein Null sein drüfen. 252 = 11111100
    if ((flags ^ 252) < 252)
    {
        fprintf(stderr, "Invalid flag: %02x\n", flags);
        exit(1);
    }

    // TODO: arbeite an frag_offset

    if (!validate_IP_PRTCL_NUM(prtcl))
    {
        exit(1); // TODO: msg to stderr.
    }

    // TODO: check the checksum

    // TODO: check the ips

    // TODO: check the options

    tIPv4Header *header = (tIPv4Header *)malloc(sizeof(tIPv4Header));

    header->get_header_str = get_ipv4_header_str;

    header->version = VR_4;
    header->IHL = IHL;
    header->TOS = TOS;
    header->total_len = total_len;
    header->id = id;
    header->flags = flags;
    header->frag_offset = frag_offset;
    header->TTL = TTL;
    header->prtcl = prtcl;
    header->checksum = checksum;
    header->src_ip = src_ip;
    header->trgt_ip = trgt_ip;

    header->options = options;

    header->len = IHL * 4;

    return header;
}

char *get_ipv4_header_str(tIPv4Header *self)
{
    char *IHL_str = format_str("%d Bytes (%d * 4)", self->IHL * 4, self->IHL);

    char *id_str = format_str("0x%04x (%d)", self->id, self->id);

    char *flag_meaning;
    switch (self->flags)
    {
    case 0:
        flag_meaning = "Fragmentation & This packet contains the last fragment.";
        break;
    case 1:
        flag_meaning = "Fragmentation & This packet contains a fragment.";
        break;
    default:
        flag_meaning = "No fragmentation.";
    }

    char *flag_meaning_ptr = (char *)malloc(strlen(flag_meaning) + 1);
    memcpy(flag_meaning_ptr, flag_meaning, strlen(flag_meaning) + 1);

    char *flags_str = format_str("0x%x (%s)", self->flags, flag_meaning_ptr);

    char *prtcl_namestr = get_IP_PRTCL_namestr(self->prtcl);
    if (!prtcl_namestr)
    {
        prtcl_namestr = "Unsupported Protocol. \
        \n\tSupported protocols are: \
        \n\t\tTCP, UDP, ICMP, and IGMP, IPv4 Encapsulation, IPv6 Encapsulation";
    }

    prtcl_namestr = format_str("%s (%d)", prtcl_namestr, self->prtcl);
    char *checksum_str = format_str("0x%04x (%s)", self->checksum, "Unverified");

    char *src_ip_str = bytes_to_ipv4_address_str(self->src_ip, "");
    char *trgt_ip_str = bytes_to_ipv4_address_str(self->trgt_ip, "");

    char *options_str = format_str(self->options && self->options->len ? "Header options present." : "No header options.");

    return format_str(
        "Packet Header:\
        \n\tVersion: 4 (IPv4)\n\tIHL (Internet Header Length): %s\
        \n\tTOS: 0x%02x\n\tTotal length: %d Bytes\
        \n\tIdentification: %s\n\tflags: %s\n\tFragment offset: %d\
        \n\tTTL (time to live): %d\n\tProtocol: %s\n\tChecksum: %s\
        \n\tSource IP: %s\n\tTarget IP: %s\
        \n\tOptions: %s\n",

        IHL_str,
        self->TOS, self->total_len,
        id_str, flags_str, self->frag_offset,
        self->TTL, prtcl_namestr, checksum_str,
        src_ip_str, trgt_ip_str,

        options_str

    );
}

tIPv4Header *parse_ipv4_header(tIPv4Protocol *self, u8 *bytes, u16 bytes_len)
{
    // TODO: Mache was wenn die Größe von u8, nicht 1 ist.

    if (bytes_len < 20)
    {
        fprintf(stderr, "Insuffecient array length: %d\n", bytes_len);
        exit(1);
    }

    u8 bindex = 0;
    u8 version = (bytes[bindex] & 0xF0) >> 4;
    u8 IHL = (bytes[bindex] & 0x0F);

    bindex++;
    u8 TOS = bytes[bindex];

    bindex++;
    u16 total_length = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u16 id = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u8 flags = (bytes[bindex] & 0xE0) >> 5;

    u16 frag_offset = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;
    u8 TTL = bytes[bindex];

    bindex++;
    u8 prtcl = bytes[bindex];

    bindex++;
    u16 checksum = bytes_to_hostu16(bytes[bindex], bytes[bindex + 1]);

    bindex += 2;

    u8 *src_ip = bytes + bindex;

    bindex += 4;
    u8 *trgt_ip = bytes + bindex;

    bindex++;
    if (bindex == bytes_len)
        return self->create_header(version, IHL, TOS, total_length, id, flags, frag_offset, TTL, prtcl, checksum, src_ip, trgt_ip, nullptr);

    tIPv4HeaderOptions *options = (tIPv4HeaderOptions *)malloc(sizeof(tIPv4HeaderOptions));
    // TODO: decode options

    return self->create_header(version, IHL, TOS, total_length, id, flags, frag_offset, TTL, prtcl, checksum, src_ip, trgt_ip, options);
}

tPDU *create_ipv4_packet(
    tIPv4Header *header,
    u8 *payload,
    u16 payload_len)
{
    if (!payload || payload_len == 0 && header->total_len != 20)
    {
        fprintf(stderr, "Payload length of zero while a mismatching total length.");
        exit(1);
    }

    tPDU *packet = malloc(sizeof(tPDU));

    packet->get_repr_str = get_ipv4_repr_str;

    packet->header = header;
    packet->data = payload;
    packet->footer = nullptr;
    packet->payload_len = header->total_len - header->len;
    packet->total_len = header->total_len;

    return packet;
}

tPDU *parse_ipv4_packet(tIPv4Protocol *self, u8 *bytes, u16 bytes_len)
{
    if (bytes_len < 20)
    {
        fprintf(stderr, "Unacceptable length of input bytes for an IPv4 packet");
        exit(1);
    }

    tIPv4Header *header = self->parse_header(self, bytes, bytes_len);

    if (bytes_len < header->total_len)
    {
        fprintf(stderr, "Length of input bytes should be greater than or equal to %d", header->total_len);
        exit(1);
    }

    tPDU *packet = create_ipv4_packet(header, bytes + header->len, bytes_len - header->len);

    return packet;
}

char *get_ipv4_repr_str(tPDU *self)
{
    tIPv4Header *header = self->header;
    return format_str(
        "IPv4 Packet:\
        \n\t%s\
        \n\tPayload: (%d Bytes) \
        \n%s\n",
        header->get_header_str(header), self->payload_len, bytes_repr(self->data, self->payload_len, "\t\t"));
}

bool validate_IP_PRTCL_NUM(u8 prtcl)
{
    switch (prtcl)
    {
    case ICMP:
    case IGMP:
    case TCP:
    case UDP:
    case IPv4_ENCAPSULATION:
    case IPv6_ENCAPSULATION:
        return true;
    default:
        return false;
    }
}

char *get_IP_PRTCL_namestr(u8 prtcl)
{

    switch (prtcl)
    {
    case TCP:
        return "TCP";
    case UDP:
        return "UDP";
    case ICMP:
        return "ICMP";
    case IGMP:
        return "IGMP";
    case IPv4_ENCAPSULATION:
        return "IPv4 Encapsulation";
    case IPv6_ENCAPSULATION:
        return "IPv6 Encapsulation";
    default:
        return "";
    }
}