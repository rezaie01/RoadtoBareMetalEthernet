#include "types.h"
#include "endian.h"

bool is_host_LE() {
    // two approaches
    // u16 one = 1;
    // return *((u8 *)&one) == 1; // when pointer casting, 
    
    u8 one_as_BE[2] = {1, 0};
    return *((u16*)one_as_BE) == 1;
}

u16 u16_to_BE(u16 n) {
    if( is_host_LE()) {
        return (n << 8) | ((n & 0xFF00) >> 8);
    }

    return n;
}

u32 u32_to_BE(u32 n) {
    if( is_host_LE()) {
        return (n << 24) | ((n & 0x0000FF00) << 8) | ((n & 0x00FF0000) >> 8 ) | ((n >> 24));
    }

    return n;
}