#include "subnets_correctly_test_support.h"
#include "endian.h"
#include "mini_network_stack/shared.h"
#include "netcalc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int test_subnets_correctly(u8 test_num, TNetworkPrefix org_net,
                           u32 requested_subnets, u32 possible_subnets,
                           u32 hosts_per_subnet, u8 new_prefix) {
  puts("---");
  printf("CHECK %d: Testing against [Subnetting %s/%d to %d subnets]\n",
         test_num,
         bytes_to_ipv4_address_str(u32_to_bytes(org_net.network_id), ""),
         org_net.prefix, requested_subnets);

  printf("HI THERE");
  char *tobe_subnetting_str;
  char *isnow_subnetting_str;
  if (requested_subnets == 1) {

    printf("HI THERE");
    tobe_subnetting_str = get_subnetting_str(org_net, requested_subnets,
                                             possible_subnets, NULL);
  } else if ((org_net.prefix < 30) && (0 <= new_prefix && new_prefix <= 30) &&
             (requested_subnets >= 1)) {

    TNetworkProfile **subnet_profiles =
        calloc(possible_subnets, sizeof(TNetworkProfile *));

    u32 subnetworkid1 = org_net.network_id;

    for (int i = 0; i < requested_subnets; i++) {
      TNetworkProfile *subnet = malloc(sizeof(TNetworkProfile));
      if (!subnet)
        return EXIT_FAILURE;

      init_network_profile(subnet, subnetworkid1 + (i * (hosts_per_subnet + 2)),
                           new_prefix);
      subnet_profiles[i] = subnet;
    }

    tobe_subnetting_str = get_subnetting_str(org_net, requested_subnets,
                                             possible_subnets, subnet_profiles);

  } else if (new_prefix > 30) { //
    tobe_subnetting_str = ERRSUBNET__NEW_PREFIX_GT_30;
  } else if (requested_subnets < 1) {
    tobe_subnetting_str = ERRSUBNET__REQUIRED_SUBNETS_LT_1;
  } else if (org_net.prefix >= 30) {
    tobe_subnetting_str = ERRSUBNET__PREFIX_GE_30;
  }

  printf("HI THERE");
  char *argv[4] = {"netcalc",
                   format_str("%s/%d",
                              bytes_to_ipv4_address_str(
                                  u32_to_bytes(org_net.network_id), ""),
                              org_net.prefix),
                   "subnets", format_str("%d", requested_subnets)};

  isnow_subnetting_str = handle_args(4, argv);

  puts("####");
  puts(isnow_subnetting_str);
  puts("####");
  bool failed = strcmp(tobe_subnetting_str, isnow_subnetting_str) != 0;

  printf("CHECK %d: Testing against [Subnetting %s/%d to %d subnets] [%s] \n",
         test_num,
         bytes_to_ipv4_address_str(u32_to_bytes(org_net.network_id), ""),
         org_net.prefix, requested_subnets, failed ? "FAILED" : "PASSED");

  // puts(tobe_subnetting_str);
  puts("---");

  if (failed) {
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
