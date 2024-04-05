#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_TINY_ENCRYPT [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_TINY_ENCRYPT = sizeof(g_Start_TINY_ENCRYPT);
const uint32_t g_SizeCode_TINY_ENCRYPT = 32;

#define PROTECT_CODE_TINY_ENCRYPT