#ifndef __PARSE_IPV4_HEADER_H__

#define __PARSE_IPV4_HEADER_H__


#include "types.h"

bool is_host_LE();
u16 u16_to_BE(u16 n);
u32 u32_to_BE(u32 n);
u16 bytes_to_hostu16(u8 b1, u8 b0);

u32 bytes_to_hostu32(u8 b3, u8 b2, u8 b1, u8 b0);
#endif