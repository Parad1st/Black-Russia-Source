#pragma once

#include <stdint.h>

#include "CFileHashed.h"

struct SMDFile
{
	char szFileLocation[128];
	uint32_t iCorrectDigestArray[4];
	unsigned char szRawLocalDigest[16];
};

class CCheckFileHash
{
	static bool IsFileValid(SMDFile* pMDFile);
public:
	static bool IsFilesValid();
};

