#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_MODELINFO_EDITABLE [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_MODELINFO_EDITABLE = sizeof(g_Start_MODELINFO_EDITABLE);
const uint32_t g_SizeCode_MODELINFO_EDITABLE = 32;

#define PROTECT_CODE_MODELINFO_EDITABLE