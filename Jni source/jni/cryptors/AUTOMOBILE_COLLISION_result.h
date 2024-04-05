#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_AUTOMOBILE_COLLISION [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_AUTOMOBILE_COLLISION = sizeof(g_Start_AUTOMOBILE_COLLISION);
const uint32_t g_SizeCode_AUTOMOBILE_COLLISION = 32;

#define PROTECT_CODE_AUTOMOBILE_COLLISION