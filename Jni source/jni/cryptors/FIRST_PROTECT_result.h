#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_FIRST_PROTECT [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_FIRST_PROTECT = sizeof(g_Start_FIRST_PROTECT);
const uint32_t g_SizeCode_FIRST_PROTECT = 32;

#define PROTECT_CODE_FIRST_PROTECT