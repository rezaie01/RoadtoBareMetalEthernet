#ifndef ___MEIN_TYPES_H__
#define ___MEIN_TYPES_H__

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t  i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef enum EtherType {
    ARP = 0x0806,
    IPv4 = 0x0800,
    IPv6 = 0x86DD,
    WoN = 0x0842,
} EtherType;
#endif