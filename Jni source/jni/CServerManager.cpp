#include "CServerManager.h"

#include <stdint.h>

const char* g_szServerNames[MAX_SERVERS] = {
	"Brilliant RP | Phantom",
	"Brilliant RP | Cullinan"

};

const CServerInstance::CServerInstanceEncrypted g_sEncryptedAddresses[MAX_SERVERS] = {
	CServerInstance::create("m1.b-rp.ru", 1, 16, 7777, false), // 1
	CServerInstance::create("m2.b-rp.ru", 1, 16, 7777, false) // 2
};