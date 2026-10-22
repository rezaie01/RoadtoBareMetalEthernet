#ifndef __NETCALC_H__
#define __NETCALC_H__

#include "types.h"

// returns an array containing 4 octals of an ip, plus the prefix {0xxx,0xxx,0xxx,0xxx, prefix}
int handle_args(int argc, char* argv[]);

u8 *str_t_ipv4(char *ipv4str);
u8 *str_t_routingprefixv4(char *ipv4str);



#endif