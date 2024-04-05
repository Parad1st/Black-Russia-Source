#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_SERVER_CHECK3 [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_SERVER_CHECK3 = sizeof(g_Start_SERVER_CHECK3);
const uint32_t g_SizeCode_SERVER_CHECK3 = 32;

#define PROTECT_CODE_SERVER_CHECK3