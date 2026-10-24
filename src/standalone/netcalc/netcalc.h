#ifndef __NETCALC_H__
#define __NETCALC_H__

#include "types.h"

// returns an array containing 4 octals of an ip, plus the prefix {0xxx,0xxx,0xxx,0xxx, prefix}
char* handle_args(int argc, char* argv[]);

u8 *str_t_ipv4(char *ipv4str);
u8 *str_t_routingprefixv4(char *ipv4str);

char *get_networkprofile_str(u8 *routingprfx_arr, u8 prfx, u32 subnetz_maske, u32 wildcard_maske, u32 netzwerk_id, u32 broadcast_ip, u8 prfx_diff, u32 min_ip, u32 max_ip, u32 usable_hosts_c);


#endif