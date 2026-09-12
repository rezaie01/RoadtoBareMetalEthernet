
#ifndef __MNS_PDU_H__ // stands for mini network stack
#define __MNS_PDU_H__ 
#include "../types.h"

typedef struct tPDU {
    void *header;
    void *footer;
    
    u8 data[];
} tPDU;

#endif