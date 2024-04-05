#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_COMPONENT_VISIBLE_INTERNAL [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_COMPONENT_VISIBLE_INTERNAL = sizeof(g_Start_COMPONENT_VISIBLE_INTERNAL);
const uint32_t g_SizeCode_COMPONENT_VISIBLE_INTERNAL = 32;

#define PROTECT_CODE_COMPONENT_VISIBLE_INTERNAL