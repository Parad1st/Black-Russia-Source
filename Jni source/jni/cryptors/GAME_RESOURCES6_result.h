#include <stdint.h>
#include "../util/armhook.h"

const uint8_t g_Start_GAME_RESOURCES6 [] = { 0x82, 0xB0,
									0x02, 0xB0,

									0x00, 0x46,
									0x00, 0x46,
									0x00, 0x46 };

const uint32_t g_SizeStart_GAME_RESOURCES6 = sizeof(g_Start_GAME_RESOURCES6);
const uint32_t g_SizeCode_GAME_RESOURCES6 = 32;

#define PROTECT_CODE_GAME_RESOURCES6