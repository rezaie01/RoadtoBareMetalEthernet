#ifndef __MSN_SHARED_H__
#define __MSN_SHARED_H__

#include "types.h"

char *bytes_repr(u8 *bytes, u32 size, char *line_prefix);
char *ether_type_to_str(u16 eth_type);

char *bytes_to_mac_str(u8 *mac, char *prefix);
char *bytes_to_ipv4_address_str(u8 *addr, char *prefix);

char *format_str(char* frmt, ...);
#endif