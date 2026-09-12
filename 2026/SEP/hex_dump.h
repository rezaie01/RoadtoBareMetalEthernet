#ifndef __HEX_DUMP_H__
#define __HEX_DUMP_H__
#include <stdint.h>
#include <stdlib.h>

#include "types.h"

void hex_dump(const u8* bytes, size_t size);
#endif