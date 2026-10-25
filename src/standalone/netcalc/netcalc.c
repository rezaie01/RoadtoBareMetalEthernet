#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "endian.h"
#include "mini_network_stack/shared.h"
#include "netcalc.h"

u64 intpowof2(u8 n) { return (u64)1 << (n); }

char *handle_args(int argc, char *argv[]) {
  if (argc > 1) {
    // Wahrscheinlich format: 192.168.10.0 hosts/subnets 50
    if (argc == 4) {
      if (strcmp("hosts", argv[2]) == 0) {
        // check if 3rd arg is parsable to a number
        char **endptr = nullptr;

        i64 hosts_c =
            strtol(argv[3], endptr,
                   10); // TODO: number parsing should drop if e.g. 12k

        if (endptr) {
          fprintf(stderr, "Invalid Argument. Expected number of hosts: %s\n",
                  argv[3]);
          exit(1);
        }

        if (hosts_c < 0 || hosts_c > intpowof2(32) - 2) {
          fprintf(stderr, "Number of hosts should fullfill:  0 <=%s<= %lu\n",
                  argv[3], intpowof2(32) - 2);
          exit(1);
        }

        u8 *ipv4_arr = str_t_ipv4(argv[1]);

        i64 actual_c = 0;
        u8 prfx_diff = 0;

        // 2 vergen
        //

        while (hosts_c != 0 && actual_c - 2 < hosts_c) {
          actual_c = intpowof2(++prfx_diff);
        }

        if (hosts_c == 0)
          prfx_diff = 1;

        u8 prfx = 32 - prfx_diff;

        u32 ip_as_num = bytes_to_hostu32(ipv4_arr[0], ipv4_arr[1], ipv4_arr[2],
                                         ipv4_arr[3]);

        u32 subnetz_maske = intpowof2(32) - intpowof2(prfx_diff);
        u32 wildcard_maske = intpowof2(prfx_diff) - 1;
        u32 netzwerk_id = ip_as_num & subnetz_maske;
        u32 broadcast_ip = ip_as_num | wildcard_maske;
        u32 min_ip = netzwerk_id + 1;
        u32 max_ip = broadcast_ip - 1;
        u32 usable_hst_c = prfx_diff < 2 ? 0
                                         : intpowof2(prfx_diff) -
                                               2; // edge-case: /32 /31 handeled

        return get_networkprofile_str(ipv4_arr, prfx, subnetz_maske,
                                      wildcard_maske, netzwerk_id, broadcast_ip,
                                      prfx_diff, min_ip, max_ip, usable_hst_c);

        // the same old same
      } else if (strcmp("subnets", argv[2]) == 0) {
        // check if 3rd arg is parsable to a number
        char **dummystrptr = nullptr;

        i64 required_subnets =
            strtol(argv[3], dummystrptr,
                   10); // TODO: number parsing should drop if e.g. 12k

        if (dummystrptr) {
          fprintf(stderr,
                  "Invalid Argument. Expected number of required subnets: %s\n",
                  argv[3]);
          exit(1);
        }

        // validate & extract the network_prefix
        u8 *ipCIDR = str_t_ipv4CIDR(argv[1]);
        TNetworkPrefix network_prefix = {
            bytes_to_hostu32(ipCIDR[0], ipCIDR[1], ipCIDR[2], ipCIDR[3]),
            ipCIDR[4]};

        if (required_subnets < 1) {
          return ERRSUBNET__REQUIRED_SUBNETS_LT_1;
        }

        if (required_subnets == 1) {
          return get_subnetting_str(network_prefix, required_subnets, 1,
                                    nullptr);
        }

        if (network_prefix.prefix >= 30) {
          return ERRSUBNET__PREFIX_GE_30;
        }

        u32 possible_subnets = 0;
        u8 powOf2 = 0;
        while (possible_subnets < required_subnets &&
               powOf2 + network_prefix.prefix < 30) {
          possible_subnets = 1 << ++powOf2;
        }

        if (powOf2 + network_prefix.prefix > 30) {
          return ERRSUBNET__NEW_PREFIX_GT_30;
        }

        TNetworkProfile **subnet_profiles =
            calloc(possible_subnets, sizeof(TNetworkProfile *));

        for (int i = 0; i < possible_subnets; i++) {
          TNetworkProfile *subnet = malloc(sizeof(TNetworkProfile));
          u8 new_prefix = network_prefix.prefix + powOf2;
          u32 numof_ips = (1 << (32 - new_prefix));
          bool init_success = init_network_profile(
              subnet, network_prefix.network_id + (i * numof_ips),
              network_prefix.prefix + powOf2);

          if (!init_success) {
            fprintf(stderr, ERR_NETWORK_PROFILE_INIT_FAILED);
            exit(1);
          }

          subnet_profiles[i] = subnet;
        }

        return get_subnetting_str(network_prefix, required_subnets,
                                  possible_subnets, subnet_profiles);
      }
    } else if (argc == 2) {

      // Routing prefix as an array
      u8 *routingprfx_arr = argc == 2 ? str_t_ipv4CIDR(argv[1]) : nullptr;

      // argc 2, and argv[1] CIDR
      if (routingprfx_arr) {
        u8 prfx = routingprfx_arr[4];

        if (prfx > 32)
          exit(1);

        u8 prfx_diff = 32 - prfx;
        u32 ip_as_num =
            bytes_to_hostu32(routingprfx_arr[0], routingprfx_arr[1],
                             routingprfx_arr[2], routingprfx_arr[3]);

        u32 subnetz_maske = intpowof2(32) - intpowof2(prfx_diff);
        u32 wildcard_maske = intpowof2(prfx_diff) - 1;
        u32 netzwerk_id = ip_as_num & subnetz_maske;
        u32 broadcast_ip = ip_as_num | wildcard_maske;
        u32 min_ip = netzwerk_id + 1;
        u32 max_ip = broadcast_ip - 1;
        u32 usable_hst_c = prfx_diff < 2 ? 0
                                         : intpowof2(prfx_diff) -
                                               2; // edge-case: /32 /31 handeled

        return get_networkprofile_str(routingprfx_arr, prfx, subnetz_maske,
                                      wildcard_maske, netzwerk_id, broadcast_ip,
                                      prfx_diff, min_ip, max_ip, usable_hst_c);
      }
    }
  }

  return "";
}

u8 *str_t_ipv4(char *ipv4str) {
  i32 *ip_arr_int =
      malloc(4 * sizeof(i32)); // use i32, so later we can catch if the
                               // original ip had octals of 0-255

  if (!ip_arr_int)
    return nullptr;

  int scan_c = sscanf(ipv4str, "%d.%d.%d.%d", ip_arr_int, ip_arr_int + 1,
                      ip_arr_int + 2, ip_arr_int + 3);

  if (scan_c != 4)
    goto invalid_ip;

  u8 *ip_arr = malloc(4 * sizeof(u8));

  if (!ip_arr)
    goto invalid_ip;

  for (int i = 0; i < 4; i++) {
    if (!(0 <= ip_arr_int[i] && ip_arr_int[i] <= 255))
      goto invalid_ip;
    else
      ip_arr[i] = (u8)ip_arr_int[i];
  }

  // passed the check, so free the ip_arr_int
  free(ip_arr_int);

  // checks if there were any trainling charachters attached to the incoming
  // argument
  char *valid_str = bytes_to_ipv4_address_str(
      ip_arr, ""); // TODO: check for malloc-results if they are null, in all
                   // of the code

  if (strlen(valid_str) != strlen(ipv4str))
    goto invalid_ip;
  else
    free(valid_str);

  return ip_arr;

invalid_ip:
  free(ip_arr); // REMEMBER: if a ptr is null, free doesn't do anything.
  free(valid_str);
  free(ip_arr_int);
  return nullptr;
}

u8 *str_t_ipv4CIDR(char *ipv4str) {
  i32 *ip_arr_int =
      malloc(5 * sizeof(i32)); // use i32, so later we can catch if the
                               // original ip had octals of 0-255

  if (!ip_arr_int)
    return nullptr;

  int scan_c = sscanf(ipv4str, "%d.%d.%d.%d/%d", ip_arr_int, ip_arr_int + 1,
                      ip_arr_int + 2, ip_arr_int + 3, ip_arr_int + 4);

  if (scan_c != 5)
    goto invalid_ip;

  u8 *ip_arr = malloc(5 * sizeof(u8));

  if (!ip_arr)
    goto invalid_ip;

  // überprüfe ob das String formatlich korrekt ist.
  if (ip_arr_int[4] < 0 || ip_arr_int[4] > 32)
    goto invalid_ip;
  else
    ip_arr[4] = (u8)ip_arr_int[4];

  for (int i = 0; i < 4; i++) {
    if (!(0 <= ip_arr_int[i] && ip_arr_int[i] <= 255))
      goto invalid_ip;
    else
      ip_arr[i] = (u8)ip_arr_int[i];
  }

  // passed the check, so free the ip_arr_int
  free(ip_arr_int);

  // checks if there were any trainling charachters attached to the incoming
  // argument
  char *valid_str = format_str("%s/%d", bytes_to_ipv4_address_str(ip_arr, ""),
                               ip_arr[4]); // TODO: check for malloc-results if
                                           // they are null, in all of the code

  if (strlen(valid_str) != strlen(ipv4str))
    goto invalid_ip;
  else
    free(valid_str);

  return ip_arr;

invalid_ip:
  free(ip_arr); // REMEMBER: if a ptr is null, free doesn't do anything.
  free(valid_str);
  free(ip_arr_int);
  return nullptr;
}

void print_netprofile(TNetworkProfile *profile) {
  puts(get_networkprofile_str(
      u32_to_bytes(profile->network_prefix.network_id),
      profile->network_prefix.prefix, profile->subnet_mask,
      profile->wildcard_mask, profile->network_prefix.network_id,
      profile->broadcast_ip, 32 - profile->network_prefix.prefix,
      profile->first_host_ip, profile->last_host_ip,
      profile->usable_hosts_count));
}

char *get_networkprofile_str(u8 *ipv4_arr, u8 prfx, u32 subnetz_maske,
                             u32 wildcard_maske, u32 netzwerk_id,
                             u32 broadcast_ip, u8 prfx_diff, u32 min_ip,
                             u32 max_ip, u32 usable_hosts_c) {
  return format_str(
      "IP-Adresse: %s\n"
      "Präfix: /%d %s\n"
      "Subnetzmaske: %s\n"
      "wildcardmaske: %s\n"
      "Netzwerk ID: %s/%d\n"
      "Broadcast IP: %s\n"
      "Min Usable IP: %s\n"
      "Max Usable IP: %s\n"
      "Anzahl der adressierbaren Hosts: %u\n",
      bytes_to_ipv4_address_str(ipv4_arr, ""), prfx,
      prfx == 0 ? "All hosts would be local" : "",
      bytes_to_ipv4_address_str(u32_to_bytes(subnetz_maske), ""),
      bytes_to_ipv4_address_str(u32_to_bytes(wildcard_maske), ""),
      bytes_to_ipv4_address_str(u32_to_bytes(netzwerk_id), ""), prfx,
      bytes_to_ipv4_address_str(u32_to_bytes(broadcast_ip), ""),
      prfx_diff < 2 ? "NA"
                    : bytes_to_ipv4_address_str(u32_to_bytes(min_ip), ""),
      prfx_diff < 2 ? "NA"
                    : bytes_to_ipv4_address_str(u32_to_bytes(max_ip), ""),
      usable_hosts_c);
}

bool init_network_profile(TNetworkProfile *network_profile, u32 network_id,
                          u8 prefix) {
  if (prefix > 32)
    return false;

  network_profile->network_prefix.network_id = network_id;
  network_profile->network_prefix.prefix = prefix;

  network_profile->wildcard_mask = (1 << (32 - prefix)) - 1;
  network_profile->subnet_mask = ~network_profile->wildcard_mask;

  network_profile->broadcast_ip = network_id | network_profile->wildcard_mask;

  network_profile->first_host_ip =
      network_profile->wildcard_mask > 2 ? network_id + 1 : 0;
  network_profile->last_host_ip = network_profile->wildcard_mask > 2
                                      ? network_profile->broadcast_ip - 1
                                      : 0;
  network_profile->usable_hosts_count = network_profile->wildcard_mask > 2
                                            ? network_profile->wildcard_mask - 1
                                            : 0;
  network_profile->isValid = true;

  return true;
}

char *get_subnetting_str(TNetworkPrefix orignal_net, u32 required_subnets,
                         u32 possible_subnets,
                         TNetworkProfile **subnet_profiles) {

  char *org_network_id =
      bytes_to_ipv4_address_str(u32_to_bytes(orignal_net.network_id), "");

  if (required_subnets == 1) {
    TNetworkProfile *net_profile = malloc(sizeof(TNetworkProfile));
    if (!init_network_profile(net_profile, orignal_net.network_id,
                              orignal_net.prefix))
      return ERR_NETWORK_PROFILE_INIT_FAILED;

    u8 n_of_hostbits = 32 - orignal_net.prefix;
    char *subnetmask_str = bytes_to_ipv4_address_str(u32_to_bytes(net_profile->subnet_mask), "");
    return format_str(
        "Requested subnets:   %d ** Thus NO CHANGE to the Original Network **\n"
        "\n"
        "Original network (unchanged):                     %s/%d\n"
        "New prefix (unchanged):                           /%d\n"
        "Subnet mask (unchanged):                          %s\n"
        "\n"
        "Usable hosts/subnet (unchanged):                  %d\n",
        required_subnets, org_network_id, orignal_net.prefix,
        orignal_net.prefix, subnetmask_str, net_profile->usable_hosts_count);
  }

  if (!subnet_profiles || !subnet_profiles[0])
    return "";

  char *new_subnetmask_str = bytes_to_ipv4_address_str(
      u32_to_bytes(subnet_profiles[0]->subnet_mask), "");
  u32 hosts_per_subnet = subnet_profiles[0]->usable_hosts_count;

  char *header =
      format_str("Requested subnets:  %d\n"
                 "\n"
                 "Original network:    %s/%d\n"
                 "New prefix:          /%d\n"
                 "Subnet mask:         %s\n"
                 "\n"
                 "Possible subnets:    %d\n"
                 "Usable hosts/subnet: %d\n",
                 required_subnets, org_network_id, orignal_net.prefix,
                 subnet_profiles[0]->network_prefix.prefix, new_subnetmask_str,
                 possible_subnets, hosts_per_subnet);

  char *total_output = format_str("%s\n\n", header);
  for (int i = 0; i < required_subnets; i++) {
    if (!subnet_profiles[i] || !subnet_profiles[0]->network_prefix.network_id ||
        !subnet_profiles[0]->broadcast_ip)
      return "";

    char *subnetstr = format_str(
        "Subnet %d\n"
        "Network:   %s\n"
        "Broadcast: %s\n",
        i + 1,
        bytes_to_ipv4_address_str(
            u32_to_bytes(subnet_profiles[i]->network_prefix.network_id), ""),
        bytes_to_ipv4_address_str(
            u32_to_bytes(subnet_profiles[i]->broadcast_ip), ""));
    total_output = format_str("%s\n%s", total_output, subnetstr);
  }

  return total_output;
}
