#ifndef __H_SUBNETS_CORRECTLY_TEST_SUPPORT__
#define __H_SUBNETS_CORRECTLY_TEST_SUPPORT__
#include "netcalc.h"
#include "types.h"
int test_subnets_correctly(u8 test_num, TNetworkPrefix org_net,

                           u32 requested_subnets, u32 possible_subnets,
                           u32 hosts_per_subnet, u8 new_prefix);
#endif
