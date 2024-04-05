#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_FPS_FIX_PROTECT [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_FPS_FIX_PROTECT = sizeof(g_Start_FPS_FIX_PROTECT);
const uint32_t g_SizeCode_FPS_FIX_PROTECT = 32;

#define PROTECT_CODE_FPS_FIX_PROTECT