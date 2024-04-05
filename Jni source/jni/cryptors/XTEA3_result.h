#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_XTEA3 [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_XTEA3 = sizeof(g_Start_XTEA3);
const uint32_t g_SizeCode_XTEA3 = 32;

#define PROTECT_CODE_XTEA3