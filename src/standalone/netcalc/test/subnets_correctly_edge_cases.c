#include "netcalc.h"
#include "subnets_correctly_test_support.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  int check_num = 1;
  int failed = 0;

  /* 192.168.10.0/24: 1 subnet needs 0 bits, the network stays as it is */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xC0A80A00, 24},
                                   1, 1, 254, 24);
  check_num++;

  /* 192.168.10.0/24: maximum split, requested == possible, /30 (2 hosts) */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xC0A80A00, 24},
                                   64, 64, 2, 30);
  check_num++;

  /* 192.168.10.0/24: one more than a power of two (9 -> 16), /28 */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xC0A80A00, 24},
                                   9, 16, 14, 28);
  check_num++;

  /* 0.0.0.0/0: network ID 0 and prefix 0 (shift by 32 trap), 1 bit -> /1 */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0x00000000, 0},
                                   2, 2, 2147483646, 1);
  check_num++;

  /* 255.255.255.0/24: last subnet's broadcast is 0xFFFFFFFF (u32 overflow) */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xFFFFFF00, 24},
                                   4, 4, 62, 26);
  check_num++;

  /* 192.168.1.8/29: smallest splittable parent (2 x /30), not starting at .0 */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xC0A80108, 29},
                                   2, 2, 2, 30);
  check_num++;

  printf("\n%s: %d test(s) failed\n", failed ? "FAILED" : "ALL PASSED", failed);
  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}