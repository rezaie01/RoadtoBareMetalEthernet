
#ifndef __MNS_PDU_H__ // stands for mini network stack
#define __MNS_PDU_H__ 
#include "../types.h"

struct tPDU;

typedef struct tPDU tPDU;

struct tPDU {
    void *header;
    void *footer;
    u8* data;
    u32 total_len;
    u32 payload_len;

    char* (*get_repr_str)(tPDU* self);
};

#endif