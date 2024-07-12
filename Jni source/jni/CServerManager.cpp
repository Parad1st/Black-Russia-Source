#include "CServerManager.h"

#include <stdint.h>

const char* g_szServerNames[MAX_SERVERS] = {
	"Black Russia | Red",
	"Black Russia | Blue"

};

const CServerInstance::CServerInstanceEncrypted g_sEncryptedAddresses[MAX_SERVERS] = {
	CServerInstance::create("red.blackrussia.online", 1, 16, 5125, false), // 1
	CServerInstance::create("blue.blackrussia.online", 1, 16, 5125, false) // 2
};
