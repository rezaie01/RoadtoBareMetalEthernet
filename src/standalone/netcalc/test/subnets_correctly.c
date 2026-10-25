#include "netcalc.h"
#include "subnets_correctly_test_support.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
  int check_num = 1;
  int failed = 0;

  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xC0A80A00, 24},
                                   6, 8, 30, 27);
  check_num++;

  /* 10.0.0.0/8: 10 subnets need 4 bits -> 16 possible, /12 */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0x0A000000, 8},
                                   10, 16, 1048574, 12);
  check_num++;

  /* 172.16.0.0/16: 5 subnets need 3 bits -> 8 possible, /19 */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xAC100000, 16},
                                   5, 8, 8190, 19);
  check_num++;

  /* 192.168.1.0/24: requested == possible, exactly 1 bit -> /25 */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xC0A80100, 24},
                                   2, 2, 126, 25);
  check_num++;

  /* 10.16.0.0/12: parent prefix not on an octet boundary, 2 bits -> /14 */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0x0A100000, 12},
                                   3, 4, 262142, 14);
  check_num++;

  /* 192.168.100.0/24: 50 subnets need 6 bits -> 64 possible, /30 (2 hosts) */
  failed += test_subnets_correctly(check_num, (TNetworkPrefix){0xC0A86400, 24},
                                   50, 64, 2, 30);
  check_num++;

  printf("\n%s: %d test(s) failed\n", failed ? "FAILED" : "ALL PASSED", failed);
  return failed > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}