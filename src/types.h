#ifndef ___MEIN_TYPES_H__
#define ___MEIN_TYPES_H__

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef enum EtherType {
    ARP = 0x0806,
    IPv4 = 0x0800,
    IPv6 = 0x86DD,
    WoN = 0x0842,
} EtherType;
#endif