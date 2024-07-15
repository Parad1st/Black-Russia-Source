#pragma once

#include <stdint.h>
#include "str_obfuscator_no_template.hpp"
#include "game/common.h"

#define MAX_HASHED_FILES	4

class CFileHashed
{
public:
	const cryptor::string_encryptor encStr;
	const uint32_t uiCorrectDigest1;
	const uint32_t uiCorrectDigest2;
	const uint32_t uiCorrectDigest3;
	const uint32_t uiCorrectDigest4;

	constexpr CFileHashed(const char* _szFile, size_t iLen, uint32_t _uiCorrectDigest1, uint32_t _uiCorrectDigest2, uint32_t _uiCorrectDigest3, uint32_t _uiCorrectDigest4) :
		encStr(cryptor::create(_szFile, iLen)), uiCorrectDigest1(_uiCorrectDigest1), uiCorrectDigest2(_uiCorrectDigest2), uiCorrectDigest3(_uiCorrectDigest3), uiCorrectDigest4(_uiCorrectDigest4) {}
};

extern const CFileHashed g_aHashedFiles[MAX_HASHED_FILES];