#include <stdio.h>
#include <stdlib.h>

#include "ippckt_ipv4.h"

bool validate_IP_PRTCL_NUM(u8 prtcl);
tIPv4Header *create_ipv4_header(
    IP_VERSION version, u8 IHL, u8 TOS, u16 total_len,
    u16 id, u8 flags, u16 frag_offset,
    u8 TTL, IP_PRTCL_NUM prtcl, u16 checksum,
    u8* src_ip,
    u8* trgt_ip,

    tIPv4HeaderOptions *options);

tIPv4Protocol *tIPv4_ctor()
{
    tIPv4Protocol *ipv4_prtcl = (tIPv4Protocol *)malloc(sizeof(tIPv4Protocol));

    ipv4_prtcl->create_header = create_ipv4_header;

    return ipv4_prtcl;
}

tIPv4Header *create_ipv4_header(
    IP_VERSION version, u8 IHL, u8 TOS, u16 total_len,
    u16 id, u8 flags, u16 frag_offset,
    u8 TTL, IP_PRTCL_NUM prtcl, u16 checksum,
    u8* src_ip,
    u8* trgt_ip,

    tIPv4HeaderOptions *options)
{
    if (version != VR_4)
    {
        fprintf(stderr, "Called create_ipv4_header with a non-4 IP version.\n");
        exit(1);
    }

    if (IHL < 5 || IHL > 12 || options->len > 40 || total_len < 20 || total_len > (64 * 1024 - 1)) // max header length = 60.  Options max length = 40
    // MAX total_len = (64 - 1)KB, dass wir es hier nicht prüfen können. Aber falls Overflow ist passiert => wird es umgerechnet => total < 20.
    {
        fprintf(stderr, "Invalid Internet header length (IHL) OR Invalid length options OR Invalid Total length: IHL: %d, Options length: %d, Total length: %d\n", IHL, options->len, total_len);
        exit(1);
    }

    // TODO: arbeite an TOS später

    // stelle sicher, dass nur die zwei erste bits Kein Null sein drüfen. 252 = 11111100
    if (flags ^ 252 < 252)
    {
        fprintf(stderr, "Invalid flag: %02x\n", flags);
        exit(1);
    }

    // TODO: arbeite an frag_offset

    if (!validate_IP_PRTCL_NUM(prtcl)) {
        exit(1); //TODO: msg to stderr.
    }

    // TODO: check the checksum

    // TODO: check the ips

    // TODO: check the options

    tIPv4Header *header = (tIPv4Header *)malloc(sizeof(tIPv4Header));
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