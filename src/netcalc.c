#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "netcalc.h"
#include "mini_network_stack/shared.h"
#include "endian.h"

u64 intpowof2(u8 n)
{
    return (u64)1 << (n);
}

int handle_args(int argc, char *argv[])
{
    if (argc > 1)
    {
        // Wahrscheinlich format: 192.168.10.0 hosts/subnets 50
        if (argc == 4)
        {
            if (strcmp("hosts", argv[2]) == 0)
            {
                // check if 3rd arg is parsable to a number
                char **endptr = nullptr;

                i64 hosts_c = strtol(argv[3], endptr, 10);

                if (endptr)
                {
                    fprintf(stderr, "Invalid Argument. Expected number of hosts: %s\n", argv[3]);
                    exit(1);
                }

                if (hosts_c < 0 || hosts_c > intpowof2(32) - 2)
                {
                    fprintf(stderr, "Number of hosts should fullfill:  0 <=%s<= %lu\n", argv[3], intpowof2(32) - 2);
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

                if (hosts_c == 0) prfx_diff = 1;


                u8 prfx = 32 - prfx_diff;

                u32 ip_as_num = bytes_to_hostu32(ipv4_arr[0], ipv4_arr[1], ipv4_arr[2], ipv4_arr[3]);

                u32 subnetz_maske = intpowof2(32) - intpowof2(prfx_diff);
                u32 wildcard_maske = intpowof2(prfx_diff) - 1;
                u32 netzwerk_id = ip_as_num & subnetz_maske;
                u32 broadcast_ip = ip_as_num | wildcard_maske;
                u32 min_ip = netzwerk_id + 1;
                u32 max_ip = broadcast_ip - 1;
                u32 usable_hst_c = prfx_diff < 2 ? 0 : intpowof2(prfx_diff) - 2; // edge-case: /32 /31 handeled

                printf(format_str("IP-Adresse: %s\n", bytes_to_ipv4_address_str(ipv4_arr, "")));
                printf("Präfix: /%d\n", prfx);
                printf(format_str("Subnetzmaske: %s\n", bytes_to_ipv4_address_str(u32_to_bytes(subnetz_maske), "")));
                printf(format_str("wildcardmaske: %s\n", bytes_to_ipv4_address_str(u32_to_bytes(wildcard_maske), "")));
                printf(format_str("Netzwerk ID: %s/%d\n", bytes_to_ipv4_address_str(u32_to_bytes(netzwerk_id), ""), prfx));
                printf(format_str("Broadcast IP: %s\n", bytes_to_ipv4_address_str(u32_to_bytes(broadcast_ip), "")));

                printf(format_str("Min Usable IP: %s\n", prfx_diff < 2 ? "NA" : bytes_to_ipv4_address_str(u32_to_bytes(min_ip), "")));
                printf(format_str("Max Usable IP: %s\n", prfx_diff < 2 ? "NA" : bytes_to_ipv4_address_str(u32_to_bytes(max_ip), "")));
                printf(format_str("Anzahl des Hosts: %u\n", usable_hst_c));

                printf("bin hier \n");

                // calc the prefix

                // the same old same
            }
            else if (strcmp("subnets", argv[2]) == 0)
            {
            }
        }
        else if (argc == 2)
        {

            // Routing prefix as an array
            u8 *routingprfx_arr = argc == 2 ? str_t_routingprefixv4(argv[1]) : nullptr;

            // argc 2, and argv[1] CIDR
            if (routingprfx_arr)
            {
                u8 prfx = routingprfx_arr[4];

                if (prfx > 32)
                    exit(1);

                u8 prfx_diff = 32 - prfx;
                u32 ip_as_num = bytes_to_hostu32(routingprfx_arr[0], routingprfx_arr[1], routingprfx_arr[2], routingprfx_arr[3]);

                u32 subnetz_maske = intpowof2(32) - intpowof2(prfx_diff);
                u32 wildcard_maske = intpowof2(prfx_diff) - 1;
                u32 netzwerk_id = ip_as_num & subnetz_maske;
                u32 broadcast_ip = ip_as_num | wildcard_maske;
                u32 min_ip = netzwerk_id + 1;
                u32 max_ip = broadcast_ip - 1;
                u32 usable_hst_c = prfx_diff < 2 ? 0 : intpowof2(prfx_diff) - 2; // edge-case: /32 /31 handeled

                printf(format_str("IP-Adresse: %s\n", bytes_to_ipv4_address_str(routingprfx_arr, "")));
                printf("Präfix: /%d\n", prfx);
                printf(format_str("Subnetzmaske: %s\n", bytes_to_ipv4_address_str(u32_to_bytes(subnetz_maske), "")));
                printf(format_str("wildcardmaske: %s\n", bytes_to_ipv4_address_str(u32_to_bytes(wildcard_maske), "")));
                printf(format_str("Netzwerk ID: %s/%d\n", bytes_to_ipv4_address_str(u32_to_bytes(netzwerk_id), ""), prfx));
                printf(format_str("Broadcast IP: %s\n", bytes_to_ipv4_address_str(u32_to_bytes(broadcast_ip), "")));

                printf(format_str("Min Usable IP: %s\n", prfx_diff < 2 ? "NA" : bytes_to_ipv4_address_str(u32_to_bytes(min_ip), "")));
                printf(format_str("Max Usable IP: %s\n", prfx_diff < 2 ? "NA" : bytes_to_ipv4_address_str(u32_to_bytes(max_ip), "")));
                printf(format_str("Anzahl des Hosts: %u\n", usable_hst_c));
            }
        }
    }

    return EXIT_SUCCESS;
}

u8 *str_t_ipv4(char *ipv4str)
{
    i32 *ip_arr_int = malloc(4 * sizeof(i32)); // use i32, so later we can catch if the original ip had octals of 0-255

    if (!ip_arr_int)
        return nullptr;

    int scan_c = sscanf(ipv4str, "%d.%d.%d.%d", ip_arr_int, ip_arr_int + 1, ip_arr_int + 2, ip_arr_int + 3);

    if (scan_c != 4)
        goto invalid_ip;

    u8 *ip_arr = malloc(4 * sizeof(u8));

    if (!ip_arr)
        goto invalid_ip;

    for (int i = 0; i < 4; i++)
    {
        if (!(0 <= ip_arr_int[i] && ip_arr_int[i] <= 255))
            goto invalid_ip;
        else
            ip_arr[i] = (u8)ip_arr_int[i];
    }

    // passed the check, so free the ip_arr_int
    free(ip_arr_int);

    // checks if there were any trainling charachters attached to the incoming argument
    char *valid_str = bytes_to_ipv4_address_str(ip_arr, ""); // TODO: check for malloc-results if they are null, in all of the code

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

u8 *str_t_routingprefixv4(char *ipv4str)
{
    i32 *ip_arr_int = malloc(5 * sizeof(i32)); // use i32, so later we can catch if the original ip had octals of 0-255

    if (!ip_arr_int)
        return nullptr;

    int scan_c = sscanf(ipv4str, "%d.%d.%d.%d/%d", ip_arr_int, ip_arr_int + 1, ip_arr_int + 2, ip_arr_int + 3, ip_arr_int + 4);

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

    for (int i = 0; i < 4; i++)
    {
        if (!(0 <= ip_arr_int[i] && ip_arr_int[i] <= 255))
            goto invalid_ip;
        else
            ip_arr[i] = (u8)ip_arr_int[i];
    }

    // passed the check, so free the ip_arr_int
    free(ip_arr_int);

    // checks if there were any trainling charachters attached to the incoming argument
    char *valid_str = format_str("%s/%d", bytes_to_ipv4_address_str(ip_arr, ""), ip_arr[4]); // TODO: check for malloc-results if they are null, in all of the code

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