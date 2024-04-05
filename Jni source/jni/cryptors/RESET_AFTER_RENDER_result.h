#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_RESET_AFTER_RENDER [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_RESET_AFTER_RENDER = sizeof(g_Start_RESET_AFTER_RENDER);
const uint32_t g_SizeCode_RESET_AFTER_RENDER = 32;

#define PROTECT_CODE_RESET_AFTER_RENDER