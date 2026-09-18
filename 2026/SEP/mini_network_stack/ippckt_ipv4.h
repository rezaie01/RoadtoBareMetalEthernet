#ifndef __MSN_IPPCKT_IPV4_H__
#define __MSN_IPPCKT_IPV4_H__

#include "types.h"
#include "pdu.h"

/* structs */
struct tIPv4Protocol;
typedef struct tIPv4Protocol tIPv4Protocol;

struct tIPv4Header;
typedef struct tIPv4Header tIPv4Header;

struct tIPv4HeaderOptions;
typedef struct tIPv4HeaderOptions tIPv4HeaderOptions;


/* enums*/
enum IP_VERSION;
typedef enum IP_VERSION {
    VR_4 = 4,
    VR_6 = 6
} IP_VERSION;

enum IP_PRTCL_NUM;
typedef enum IP_PRTCL_NUM {
    ICMP = (u8)1,
    IGMP = (u8)2,
    TCP = (u8)6,
    UDP = (u8)17,

    IPv4_ENCAPSULATION = (u8)4,
    IPv6_ENCAPSULATION = (u8)41
} IP_PRTCL_NUM;

struct tIPv4Protocol
{
    tPDU *packet;

    int (*decode)(tIPv4Protocol *self, u8 *bytes, u16 bytes_len);
    u8 *(*encode)(const tIPv4Protocol *self);

    tIPv4Header *(*create_header)(
        IP_VERSION version, u8 IHL, u8 TOS, u16 total_len, 
        u16 id, u8 flags, u16 frag_offset,
        u8 TTL, IP_PRTCL_NUM prtcl, u16 checksum, 
        u8* src_ip, 
        u8* trgt_ip,

        tIPv4HeaderOptions* options
    );
};

struct tIPv4Header
{
    IP_VERSION version;
    u8 IHL;
    u8 TOS;
    u16 total_len;

    u16 id;
    u8 flags;
    u16 frag_offset;

    u8 TTL;
    IP_PRTCL_NUM prtcl;
    u16 checksum;

    u8* trgt_ip;
    u8* src_ip;

    tIPv4HeaderOptions* options;
    
    u16 len;

    char *(*get_header_str)(tIPv4Header *self);
};


struct tIPv4HeaderOptions {
    // TODO: fülle dies aus.
    u8 len;
};


tIPv4Protocol *tIPv4_ctor();
bool validate_IP_PRTCL_NUM(u8 prtcl);


#endif