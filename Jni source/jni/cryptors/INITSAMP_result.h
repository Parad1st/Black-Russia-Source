#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_INITSAMP [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_INITSAMP = sizeof(g_Start_INITSAMP);
const uint32_t g_SizeCode_INITSAMP = 32;

#define PROTECT_CODE_INITSAMP