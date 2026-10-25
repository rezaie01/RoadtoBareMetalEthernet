#ifndef __NETCALC_H__
#define __NETCALC_H__

#define ERRSUBNET__REQUIRED_SUBNETS_LT_1 "ERROR: Number of subnets should be >= 1\n"
#define ERRSUBNET__PREFIX_GE_30 "ERROR: A network of /30 or smaller cannot be subnetted\n"
#define ERRSUBNET__NEW_PREFIX_GT_30 "ERROR: Cannot create subnets smaller than /30"
#define ERR_NETWORK_PROFILE_INIT_FAILED "ERROR: Initialization of The Network Profile failed."
#include "types.h"

struct TNetworkPrefix;
typedef struct TNetworkPrefix TNetworkPrefix;
struct TNetworkPrefix {
  u32 network_id;
  u32 prefix;
};

struct TNetworkProfile;
typedef struct TNetworkProfile TNetworkProfile;

struct TNetworkProfile {
  TNetworkPrefix network_prefix;
  u32 subnet_mask;
  u32 wildcard_mask;
  u32 broadcast_ip;
  u32 first_host_ip;
  u32 last_host_ip;
  u32 usable_hosts_count;
  bool isValid;
};

bool init_network_profile(TNetworkProfile *network_profile, u32 network_id,
                          u8 prefix);

// returns an array containing 4 octals of an ip, plus the prefix
// {0xxx,0xxx,0xxx,0xxx, prefix}
char *handle_args(int argc, char *argv[]);

u8 *str_t_ipv4(char *ipv4str);
u8 *str_t_ipv4CIDR(char *ipv4str);

char *get_networkprofile_str(u8 *routingprfx_arr, u8 prfx, u32 subnetz_maske,
                             u32 wildcard_maske, u32 netzwerk_id,
                             u32 broadcast_ip, u8 prfx_diff, u32 min_ip,
                             u32 max_ip, u32 usable_hosts_c);

void print_netprofile(TNetworkProfile *profile);
char *get_subnetting_str(TNetworkPrefix orignal_net, u32 required_subnets,
                         u32 possible_subnets,
                         TNetworkProfile **subnet_profiles);

#endif