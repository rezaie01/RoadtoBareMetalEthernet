#include "netcalc.h"
#include <stdio.h>
#include <string.h>

int main() {
  u8 failed = 0;
  u8 prefailed = 0;

  printf("Test against `192.168.10.10/20`");
  failed += /* A normal IP */
      strcmp(handle_args(2, (char *[2]){"netcalc", "192.168.10.10/20"}),
             get_networkprofile_str(str_t_ipv4("192.168.10.10"), 20, 0xFFFFF000,
                                    0x00000FFF, 0xC0A80000, 0xC0A80FFF, 12,
                                    0xC0A80001, 0xC0A80FFE, 4094));
  printf("Test against `192.168.10.10/20`: %s", failed == prefailed? "PASSED": "FAILED");
  prefailed = failed;

  
  printf("Test 1 against `163.177.121.157/11`\n");
  failed += /* Test 1: 163.177.121.157/11 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "163.177.121.157/11"}),
              get_networkprofile_str(str_t_ipv4("163.177.121.157"), 11, 0xFFE00000,
                                     0x001FFFFF, 0xA3A00000, 0xA3BFFFFF, 21,
                                     0xA3A00001, 0xA3BFFFFE, 2097150)) != 0);
  printf("Test 1 against `163.177.121.157/11`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 2 against `6.103.26.209/16`\n");
  failed += /* Test 2: 6.103.26.209/16 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "6.103.26.209/16"}),
              get_networkprofile_str(str_t_ipv4("6.103.26.209"), 16, 0xFFFF0000,
                                     0x0000FFFF, 0x06670000, 0x0667FFFF, 16,
                                     0x06670001, 0x0667FFFE, 65534)) != 0);
  printf("Test 2 against `6.103.26.209/16`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 3 against `62.177.59.144/15`\n");
  failed += /* Test 3: 62.177.59.144/15 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "62.177.59.144/15"}),
              get_networkprofile_str(str_t_ipv4("62.177.59.144"), 15, 0xFFFE0000,
                                     0x0001FFFF, 0x3EB00000, 0x3EB1FFFF, 17,
                                     0x3EB00001, 0x3EB1FFFE, 131070)) != 0);
  printf("Test 3 against `62.177.59.144/15`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 4 against `173.60.45.109/25`\n");
  failed += /* Test 4: 173.60.45.109/25 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "173.60.45.109/25"}),
              get_networkprofile_str(str_t_ipv4("173.60.45.109"), 25, 0xFFFFFF80,
                                     0x0000007F, 0xAD3C2D00, 0xAD3C2D7F, 7,
                                     0xAD3C2D01, 0xAD3C2D7E, 126)) != 0);
  printf("Test 4 against `173.60.45.109/25`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 5 against `22.65.159.130/26`\n");
  failed += /* Test 5: 22.65.159.130/26 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "22.65.159.130/26"}),
              get_networkprofile_str(str_t_ipv4("22.65.159.130"), 26, 0xFFFFFFC0,
                                     0x0000003F, 0x16419F80, 0x16419FBF, 6,
                                     0x16419F81, 0x16419FBE, 62)) != 0);
  printf("Test 5 against `22.65.159.130/26`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 6 against `108.3.17.153/9`\n");
  failed += /* Test 6: 108.3.17.153/9 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "108.3.17.153/9"}),
              get_networkprofile_str(str_t_ipv4("108.3.17.153"), 9, 0xFF800000,
                                     0x007FFFFF, 0x6C000000, 0x6C7FFFFF, 23,
                                     0x6C000001, 0x6C7FFFFE, 8388606)) != 0);
  printf("Test 6 against `108.3.17.153/9`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 7 against `7.160.202.110/10`\n");
  failed += /* Test 7: 7.160.202.110/10 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "7.160.202.110/10"}),
              get_networkprofile_str(str_t_ipv4("7.160.202.110"), 10, 0xFFC00000,
                                     0x003FFFFF, 0x07800000, 0x07BFFFFF, 22,
                                     0x07800001, 0x07BFFFFE, 4194302)) != 0);
  printf("Test 7 against `7.160.202.110/10`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 8 against `129.94.246.209/27`\n");
  failed += /* Test 8: 129.94.246.209/27 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "129.94.246.209/27"}),
              get_networkprofile_str(str_t_ipv4("129.94.246.209"), 27, 0xFFFFFFE0,
                                     0x0000001F, 0x815EF6C0, 0x815EF6DF, 5,
                                     0x815EF6C1, 0x815EF6DE, 30)) != 0);
  printf("Test 8 against `129.94.246.209/27`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 9 against `50.231.6.41/30`\n");
  failed += /* Test 9: 50.231.6.41/30 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "50.231.6.41/30"}),
              get_networkprofile_str(str_t_ipv4("50.231.6.41"), 30, 0xFFFFFFFC,
                                     0x00000003, 0x32E70628, 0x32E7062B, 2,
                                     0x32E70629, 0x32E7062A, 2)) != 0);
  printf("Test 9 against `50.231.6.41/30`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 10 against `139.129.72.246/21`\n");
  failed += /* Test 10: 139.129.72.246/21 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "139.129.72.246/21"}),
              get_networkprofile_str(str_t_ipv4("139.129.72.246"), 21, 0xFFFFF800,
                                     0x000007FF, 0x8B814800, 0x8B814FFF, 11,
                                     0x8B814801, 0x8B814FFE, 2046)) != 0);
  printf("Test 10 against `139.129.72.246/21`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 11 against `56.110.203.224/22`\n");
  failed += /* Test 11: 56.110.203.224/22 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "56.110.203.224/22"}),
              get_networkprofile_str(str_t_ipv4("56.110.203.224"), 22, 0xFFFFFC00,
                                     0x000003FF, 0x386EC800, 0x386ECBFF, 10,
                                     0x386EC801, 0x386ECBFE, 1022)) != 0);
  printf("Test 11 against `56.110.203.224/22`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 12 against `207.54.213.139/8`\n");
  failed += /* Test 12: 207.54.213.139/8 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "207.54.213.139/8"}),
              get_networkprofile_str(str_t_ipv4("207.54.213.139"), 8, 0xFF000000,
                                     0x00FFFFFF, 0xCF000000, 0xCFFFFFFF, 24,
                                     0xCF000001, 0xCFFFFFFE, 16777214)) != 0);
  printf("Test 12 against `207.54.213.139/8`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 13 against `194.65.51.11/13`\n");
  failed += /* Test 13: 194.65.51.11/13 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "194.65.51.11/13"}),
              get_networkprofile_str(str_t_ipv4("194.65.51.11"), 13, 0xFFF80000,
                                     0x0007FFFF, 0xC2400000, 0xC247FFFF, 19,
                                     0xC2400001, 0xC247FFFE, 524286)) != 0);
  printf("Test 13 against `194.65.51.11/13`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 14 against `39.205.129.48/14`\n");
  failed += /* Test 14: 39.205.129.48/14 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "39.205.129.48/14"}),
              get_networkprofile_str(str_t_ipv4("39.205.129.48"), 14, 0xFFFC0000,
                                     0x0003FFFF, 0x27CC0000, 0x27CFFFFF, 18,
                                     0x27CC0001, 0x27CFFFFE, 262142)) != 0);
  printf("Test 14 against `39.205.129.48/14`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 15 against `195.116.89.238/18`\n");
  failed += /* Test 15: 195.116.89.238/18 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "195.116.89.238/18"}),
              get_networkprofile_str(str_t_ipv4("195.116.89.238"), 18, 0xFFFFC000,
                                     0x00003FFF, 0xC3744000, 0xC3747FFF, 14,
                                     0xC3744001, 0xC3747FFE, 16382)) != 0);
  printf("Test 15 against `195.116.89.238/18`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 16 against `91.230.18.142/19`\n");
  failed += /* Test 16: 91.230.18.142/19 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "91.230.18.142/19"}),
              get_networkprofile_str(str_t_ipv4("91.230.18.142"), 19, 0xFFFFE000,
                                     0x00001FFF, 0x5BE60000, 0x5BE61FFF, 13,
                                     0x5BE60001, 0x5BE61FFE, 8190)) != 0);
  printf("Test 16 against `91.230.18.142/19`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 17 against `141.82.136.241/17`\n");
  failed += /* Test 17: 141.82.136.241/17 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "141.82.136.241/17"}),
              get_networkprofile_str(str_t_ipv4("141.82.136.241"), 17, 0xFFFF8000,
                                     0x00007FFF, 0x8D528000, 0x8D52FFFF, 15,
                                     0x8D528001, 0x8D52FFFE, 32766)) != 0);
  printf("Test 17 against `141.82.136.241/17`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 18 against `212.83.221.50/28`\n");
  failed += /* Test 18: 212.83.221.50/28 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "212.83.221.50/28"}),
              get_networkprofile_str(str_t_ipv4("212.83.221.50"), 28, 0xFFFFFFF0,
                                     0x0000000F, 0xD453DD30, 0xD453DD3F, 4,
                                     0xD453DD31, 0xD453DD3E, 14)) != 0);
  printf("Test 18 against `212.83.221.50/28`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 19 against `11.187.37.153/29`\n");
  failed += /* Test 19: 11.187.37.153/29 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "11.187.37.153/29"}),
              get_networkprofile_str(str_t_ipv4("11.187.37.153"), 29, 0xFFFFFFF8,
                                     0x00000007, 0x0BBB2598, 0x0BBB259F, 3,
                                     0x0BBB2599, 0x0BBB259E, 6)) != 0);
  printf("Test 19 against `11.187.37.153/29`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("Test 20 against `167.202.212.21/23`\n");
  failed += /* Test 20: 167.202.212.21/23 */
      (strcmp(handle_args(2, (char *[2]){"netcalc", "167.202.212.21/23"}),
              get_networkprofile_str(str_t_ipv4("167.202.212.21"), 23, 0xFFFFFE00,
                                     0x000001FF, 0xA7CAD400, 0xA7CAD5FF, 9,
                                     0xA7CAD401, 0xA7CAD5FE, 510)) != 0);
  printf("Test 20 against `167.202.212.21/23`: %s\n",
         failed == prefailed ? "PASSED" : "FAILED");
  prefailed = failed;
 
  printf("\n%s: %d test(s) failed\n", failed ? "FAILED" : "ALL PASSED", failed);

  return failed;
}
