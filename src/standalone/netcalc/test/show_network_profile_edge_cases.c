#include "netcalc.h"
#include <stdio.h>
#include <string.h>

/* Expected "usable hosts" for /31 and /32. Both print min/max as "NA"
 * (prfx_diff < 2), but the host count is still printed.
 * RFC 3021: /31 -> 2 usable, /32 -> 1 usable.
 * Change these two if your netcalc uses another convention (e.g. 0 and 0). */
#define HOSTS_P31 0u
#define HOSTS_P32 0u

/* Same logging pattern as before, wrapped so each case is one call. */
#define CHECK(why, ip, cidr, prfx, mask, wild, nw, bc, diff, min, max, hosts)  \
  do {                                                                         \
    to_be = get_networkprofile_str(str_t_ipv4(ip), prfx, mask, wild, nw, bc,   \
                                   diff, min, max, hosts);                     \
    is_now = handle_args(2, (char *[2]){"netcalc", cidr});                     \
    printf("Test against `%s` (%s)\n", cidr, why);                             \
    failed += (strcmp(is_now, to_be) != 0);                                    \
    if (failed != prefailed) {                                                 \
      puts("");                                                                \
      printf("to_be vs. is_now: \n %s\n---\n%s\n", to_be, is_now);             \
    }                                                                          \
    printf("Test against `%s`: %s\n", cidr,                                    \
           failed == prefailed ? "PASSED" : "FAILED");                         \
    prefailed = failed;                                                        \
  } while (0)

int main() {
  u8 failed = 0;
  u8 prefailed = 0;

  char *to_be;
  char *is_now;

  /* ---- /0: shift by 32 is undefined behaviour in C, 2^32-2 hosts ---- */
  CHECK("/0 lowest IP", "0.0.0.0", "0.0.0.0/0", 0, 0x00000000, 0xFFFFFFFF,
        0x00000000, 0xFFFFFFFF, 32, 0x00000001, 0xFFFFFFFE, 4294967294u);
  CHECK("/0 highest IP", "255.255.255.255", "255.255.255.255/0", 0, 0x00000000,
        0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 32, 0x00000001, 0xFFFFFFFE,
        4294967294u);

  /* ---- /1: top bit decides the network (sign-extension traps) ---- */
  CHECK("/1 upper half", "128.0.0.1", "128.0.0.1/1", 1, 0x80000000, 0x7FFFFFFF,
        0x80000000, 0xFFFFFFFF, 31, 0x80000001, 0xFFFFFFFE, 2147483646u);
  CHECK("/1 lower half, last IP", "127.255.255.255", "127.255.255.255/1", 1,
        0x80000000, 0x7FFFFFFF, 0x00000000, 0x7FFFFFFF, 31, 0x00000001,
        0x7FFFFFFE, 2147483646u);

  /* ---- /8 at both ends of the address space ---- */
  CHECK("/8 loopback", "127.0.0.1", "127.0.0.1/8", 8, 0xFF000000, 0x00FFFFFF,
        0x7F000000, 0x7FFFFFFF, 24, 0x7F000001, 0x7FFFFFFE, 16777214u);
  CHECK("/8 all zero IP", "0.0.0.0", "0.0.0.0/8", 8, 0xFF000000, 0x00FFFFFF,
        0x00000000, 0x00FFFFFF, 24, 0x00000001, 0x00FFFFFE, 16777214u);
  CHECK("/8 all ones IP", "255.255.255.255", "255.255.255.255/8", 8, 0xFF000000,
        0x00FFFFFF, 0xFF000000, 0xFFFFFFFF, 24, 0xFF000001, 0xFFFFFFFE,
        16777214u);

  /* ---- prefixes that don't fall on an octet boundary ---- */
  CHECK("/4 multicast range", "224.0.0.1", "224.0.0.1/4", 4, 0xF0000000,
        0x0FFFFFFF, 0xE0000000, 0xEFFFFFFF, 28, 0xE0000001, 0xEFFFFFFE,
        268435454u);
  CHECK("/9 second octet crosses the 128 bit", "10.255.255.255",
        "10.255.255.255/9", 9, 0xFF800000, 0x007FFFFF, 0x0A800000, 0x0AFFFFFF,
        23, 0x0A800001, 0x0AFFFFFE, 8388606u);
  CHECK("/12 top of 172.16.0.0/12", "172.31.255.255", "172.31.255.255/12", 12,
        0xFFF00000, 0x000FFFFF, 0xAC100000, 0xAC1FFFFF, 20, 0xAC100001,
        0xAC1FFFFE, 1048574u);
  CHECK("/16 IP is the broadcast", "192.168.255.255", "192.168.255.255/16", 16,
        0xFFFF0000, 0x0000FFFF, 0xC0A80000, 0xC0A8FFFF, 16, 0xC0A80001,
        0xC0A8FFFE, 65534u);

  /* ---- /24: IP sits on the network, broadcast and max-usable boundaries
   * ----
   */
  CHECK("/24 IP is the network address", "192.168.1.0", "192.168.1.0/24", 24,
        0xFFFFFF00, 0x000000FF, 0xC0A80100, 0xC0A801FF, 8, 0xC0A80101,
        0xC0A801FE, 254u);
  CHECK("/24 IP is the broadcast address", "192.168.1.255", "192.168.1.255/24",
        24, 0xFFFFFF00, 0x000000FF, 0xC0A80100, 0xC0A801FF, 8, 0xC0A80101,
        0xC0A801FE, 254u);
  CHECK("/24 IP is the max usable", "192.168.1.254", "192.168.1.254/24", 24,
        0xFFFFFF00, 0x000000FF, 0xC0A80100, 0xC0A801FF, 8, 0xC0A80101,
        0xC0A801FE, 254u);

  /* ---- /30: smallest prefix that still shows min/max (diff == 2) ---- */
  CHECK("/30 normal", "192.168.1.5", "192.168.1.5/30", 30, 0xFFFFFFFC,
        0x00000003, 0xC0A80104, 0xC0A80107, 2, 0xC0A80105, 0xC0A80106, 2u);
  CHECK("/30 all zero IP", "0.0.0.0", "0.0.0.0/30", 30, 0xFFFFFFFC, 0x00000003,
        0x00000000, 0x00000003, 2, 0x00000001, 0x00000002, 2u);
  CHECK("/30 all ones IP", "255.255.255.255", "255.255.255.255/30", 30,
        0xFFFFFFFC, 0x00000003, 0xFFFFFFFC, 0xFFFFFFFF, 2, 0xFFFFFFFD,
        0xFFFFFFFE, 2u);

  /* ---- /31: prfx_diff == 1 -> min/max print "NA" ---- */
  CHECK("/31 even IP", "192.168.1.0", "192.168.1.0/31", 31, 0xFFFFFFFE,
        0x00000001, 0xC0A80100, 0xC0A80101, 1, 0xC0A80100, 0xC0A80101,
        HOSTS_P31);
  CHECK("/31 odd IP", "192.168.1.1", "192.168.1.1/31", 31, 0xFFFFFFFE,
        0x00000001, 0xC0A80100, 0xC0A80101, 1, 0xC0A80100, 0xC0A80101,
        HOSTS_P31);
  CHECK("/31 all zero IP", "0.0.0.0", "0.0.0.0/31", 31, 0xFFFFFFFE, 0x00000001,
        0x00000000, 0x00000001, 1, 0x00000000, 0x00000001, HOSTS_P31);
  CHECK("/31 all ones IP", "255.255.255.255", "255.255.255.255/31", 31,
        0xFFFFFFFE, 0x00000001, 0xFFFFFFFE, 0xFFFFFFFF, 1, 0xFFFFFFFE,
        0xFFFFFFFF, HOSTS_P31);

  /* ---- /32: prfx_diff == 0 -> "NA"; 2^0 - 2 would underflow a u32 ---- */
  CHECK("/32 normal host", "192.168.1.1", "192.168.1.1/32", 32, 0xFFFFFFFF,
        0x00000000, 0xC0A80101, 0xC0A80101, 0, 0xC0A80101, 0xC0A80101,
        HOSTS_P32);
  CHECK("/32 all zero IP", "0.0.0.0", "0.0.0.0/32", 32, 0xFFFFFFFF, 0x00000000,
        0x00000000, 0x00000000, 0, 0x00000000, 0x00000000, HOSTS_P32);
  CHECK("/32 all ones IP", "255.255.255.255", "255.255.255.255/32", 32,
        0xFFFFFFFF, 0x00000000, 0xFFFFFFFF, 0xFFFFFFFF, 0, 0xFFFFFFFF,
        0xFFFFFFFF, HOSTS_P32);

  printf("\n%s: %d test(s) failed\n", failed ? "FAILED" : "ALL PASSED", failed);
  return failed;
}
