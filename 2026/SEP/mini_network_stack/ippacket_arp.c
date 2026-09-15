
#include <stdlib.h>
#include <string.h>
#include <stdio.h> // TODO: output only if debugging

#include "../endian.h"
#include "shared.h"

#include "ippacket_arp.h"

void set_arp_header(tARPPacket *self, tIPARPHeader *header);
const tIPARPHeader *get_arp_header(const tARPPacket *self);
tIPARPHeader *create_arp_header(u16 hardware_type, u16 prtcl_type, u8 hw_size, u8 prtcl_size, u16 operation, u8 *source_mac, u8 *source_addr, u8 *target_mac, u8 *target_addr);
tIPARPHeader *parse_arp_header(tARPPacket *self, u8 *bytes, u16 bytes_len);
char *get_arp_header_str(tIPARPHeader *self);

// === ARP PACKET METHODS ===
char *get_arp_pkt_repr_str(tPDU *self);
tPDU *create_arp_packet(tIPARPHeader *header);
tARPPacket *parse_arp(tARPPacket *self, u8 *bytes, u16 bytes_len);

tARPPacket *tARPPacket_ctor()
{
    tARPPacket *arp_pckt = (tARPPacket *)malloc(sizeof(tARPPacket));
    arp_pckt->set_header = set_arp_header;
    arp_pckt->get_header = get_arp_header;

    arp_pckt->create_header = create_arp_header;
    arp_pckt->parse_header = parse_arp_header;

    arp_pckt->create_packet = create_arp_packet;
    arp_pckt->parse = parse_arp;

    return arp_pckt;
};

// TODO: Verbessere das Formatieren, um jede Zeile indentation bekommt.
char *get_arp_header_str(tIPARPHeader *self)
{
    char title[] = "ARP Header:";

    char *operation = NULL;

    switch (self->operation)
    {
    case REQUEST:
        operation = "1 (Request)";
        break;
    case REPLY:
        operation = "2 (Reply)";
        break;
    default:
        operation = "Other than 'Request' or 'Reply'";
    }

    if (self->hardware_type != 1 || self->prtcl_type != IPv4)
    {
        char format_hwtstr[] = "Unsupporeted hardware, code: 0x%04x";
        char *hw_type_str = malloc((strlen(format_hwtstr)) + 1);
        snprintf(hw_type_str, strlen(format_hwtstr) + sizeof(char), format_hwtstr, self->hardware_type);

        char format_prtcltstr[] = "Unsupporeted protocol, code: 0x%04x";
        char *prtcl_type_str = malloc((strlen(format_prtcltstr)) + 1);
        snprintf(prtcl_type_str, strlen(format_prtcltstr) + sizeof(char), format_prtcltstr, self->prtcl_type);

        char *final_str_format = "%s\n\tHardware type: %s\n\tProtocol type: %s\n\tHardware size: %d\n\tProtocol size: %d\n\tOperation code: %s\n\tSource hardware address: 0x%x\n\tSource protocol address: 0x%x\n\tTarget hardware address: 0x%x\n\fTarget protocol address: 0x%x\n";

        u16 final_size = snprintf(NULL, 0, final_str_format, title, hw_type_str, prtcl_type_str, self->hw_size, self->prtcl_size, operation, self->source_mac, self->source_addr, self->target_mac, self->target_addr);
        char *final_str = (char *)malloc((final_size + 1) * sizeof(char));
        snprintf(final_str, (final_size + 1) * sizeof(char), final_str_format, title, hw_type_str, prtcl_type_str, self->hw_size, self->prtcl_size, operation, self->source_mac, self->source_addr, self->target_mac, self->target_addr);
    }

    char *hw_type_str = "Ethernet (1)";
    char *prtcl_type_str = "IPv4 (0x0800)";

    u8 *sm = self->source_mac;
    u8 *sa = self->source_addr;
    u8 *tm = self->target_mac;
    u8 *ta = self->target_addr;

    char *src_mac = bytes_to_mac_str(self->source_mac, "Source MAC address: ");
    char *trgt_mac = bytes_to_mac_str(self->target_mac, "Target MAC address: ");

    char *src_addr = bytes_to_ipv4_address_str(self->source_addr, "Source IP address: ");
    char *trgt_addr = bytes_to_ipv4_address_str(self->target_addr, "Target IP address: ");

    char *final_str_format = "%s\n\tHardware type: %s\n\tProtocol type: %s\n\tHardware size: %d\n\tProtocol size: %d\n\tOperation code: %s\n\t%s\n\t%s\n\t%s\n\t%s\n";

    u16 final_size = snprintf(NULL, 0, final_str_format, title, hw_type_str, prtcl_type_str, self->hw_size, self->prtcl_size, operation, src_mac, src_addr, trgt_mac, trgt_addr);
    char *final_str = (char *)malloc((final_size + 1) * sizeof(char));
    snprintf(final_str, (final_size + 1) * sizeof(char), final_str_format, title, hw_type_str, prtcl_type_str, self->hw_size, self->prtcl_size, operation, src_mac, src_addr, trgt_mac, trgt_addr);

    return final_str;
}

// TODO: set_header(u8* bytes, bytes_len) implement
void set_arp_header(tARPPacket *self, tIPARPHeader *header)
{
    if (!self->packet)
    {
        self->packet = (tPDU *)malloc(sizeof(tPDU));
    }

    self->packet->header = header;
}

const tIPARPHeader *get_arp_header(const tARPPacket *self)
{
    const tIPARPHeader *header = (tIPARPHeader *)self->packet->header;
    return header;
}

tIPARPHeader *create_arp_header(
    u16 hardware_type, u16 prtcl_type,
    u8 hw_size, u8 prtcl_size,

    u16 operation,
    u8 *source_mac, u8 *source_addr,

    u8 *target_mac, u8 *target_addr)
{
    tIPARPHeader *pkt_header = (tIPARPHeader *)malloc(sizeof(tIPARPHeader));
    pkt_header->get_arp_header_str = get_arp_header_str;

    pkt_header->hardware_type = hardware_type;
    pkt_header->prtcl_type = prtcl_type;

    pkt_header->hw_size = hw_size;
    pkt_header->prtcl_size = prtcl_size;

    pkt_header->operation = operation;

    pkt_header->source_mac = (u8 *)malloc(sizeof(u8) * hw_size);
    pkt_header->source_addr = (u8 *)malloc(sizeof(u8) * prtcl_size);
    memcpy(pkt_header->source_mac, source_mac, hw_size);
    memcpy(pkt_header->source_addr, source_addr, prtcl_size);

    pkt_header->target_mac = (u8 *)malloc(sizeof(u8) * hw_size);
    pkt_header->target_addr = (u8 *)malloc(sizeof(u8) * prtcl_size);
    memcpy(pkt_header->target_mac, target_mac, hw_size);
    memcpy(pkt_header->target_addr, target_addr, prtcl_size);

    pkt_header->len = 6 + 2 * hw_size + 2 * prtcl_size; // 6 für hardware-/address-type & their sizes & operation code.
    return pkt_header;
}

// array of header bytes or the whole frame bytes.
tIPARPHeader *parse_arp_header(tARPPacket *self, u8 *bytes, u16 bytes_len)
{
    // make sure size is 1B
    if (sizeof(bytes[1]) != 1)
        exit(1); // TODO: improve the exits.

    if (bytes_len >= 28) // 28 Bytes for IPv4 ARP
    {
        u16 hw_type = bytes_to_hostu16(bytes[0], bytes[1]);
        u16 prtcl_type = bytes_to_hostu16(bytes[2], bytes[3]);

        if (hw_type != 1 || prtcl_type != IPv4)
        {
            fprintf(stderr, "Unsupported ARP Hardware/Protocol: \nHardware type: 0x%04x\nProtocol type: 0x%04x\n", hw_type, prtcl_type);
            exit(1);
        }

        u8 hw_size = bytes[4];
        u8 prtcl_size = bytes[5];

        if (hw_size != 6 || prtcl_size != 4)
        {
            fprintf(stderr, "Unsupported ARP Hardware/Protocol address size: \nHardware size: %d\nProtocol size: %d\n", hw_size, prtcl_size);
            exit(1);
        }

        u16 operation_code = bytes_to_hostu16(bytes[6], bytes[7]);

        u8 *src_mac = bytes + 8;
        u8 *src_addr = bytes + 14;

        u8 *trgt_mac = bytes + 18;
        u8 *trgt_addr = bytes + 24;

        return self->create_header(
            hw_type, prtcl_type,
            hw_size, prtcl_size,
            operation_code,
            src_mac, src_addr,
            trgt_mac, trgt_addr);
    }
    exit(1);
}

// === ARP Packet ===
// Obwohl ARP Packet hat keine Payload oder Footer. Nur um die Gründstruktur jedes TCP/IP Level zu folgen.

tARPPacket *parse_arp(tARPPacket *self, u8 *bytes, u16 bytes_len)
{
    tIPARPHeader *header = self->parse_header(self, bytes, bytes_len);

    tARPPacket *pkt_obj = tARPPacket_ctor();
    pkt_obj->packet = self->create_packet(header);

    return pkt_obj;
}

tPDU *create_arp_packet(tIPARPHeader *header)
{
    tPDU *arp_pkt = (tPDU *)malloc(sizeof(tPDU));

    arp_pkt->get_repr_str = get_arp_pkt_repr_str;

    arp_pkt->header = header;

    arp_pkt->payload_len = 0;
    arp_pkt->total_len = header->len;
    arp_pkt->footer = nullptr;

    arp_pkt->data = nullptr;

    return arp_pkt;
}

char *get_arp_pkt_repr_str(tPDU *self)
{
    tIPARPHeader *header = (tIPARPHeader *)self->header;

    char *final_str_format = "Address Resolution Protocol Packet:\n\t%s\n";
    u16 final_size = snprintf(NULL, 0, final_str_format, header->get_arp_header_str(header));
    char *final_str = (char *)malloc((final_size + 1) * sizeof(char));
    snprintf(final_str, final_size * 1, final_str_format, header->get_arp_header_str(header));

    return final_str;
}
