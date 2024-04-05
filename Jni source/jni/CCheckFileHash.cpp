#include "main.h"
#include "vendor/hash/md5.h"
#include "game/common.h"
#include "CCheckFileHash.h"

//const cryptor::string_encryptor encError = cryptor::create("CRC32 failed: \'%s\'.", 20);

#include "cryptors/CHECK_HASH1_result.h"

bool CCheckFileHash::IsFileValid(SMDFile* pMDFile)
{
	PROTECT_CODE_CHECK_HASH1;

	FILE* fLocalFile;
	fLocalFile = fopen(pMDFile->szFileLocation, "r");
	int x;
	unsigned char szBuffer[1024];

	if (!fLocalFile)
	{
		return false;
	}

	MD5_CTX mdCTX;
	MD5Init(&mdCTX);
	while ((x = fread(szBuffer, 1, sizeof(szBuffer), fLocalFile) > 0))
	{
		MD5Update(&mdCTX, szBuffer, x);
	}

	MD5Final(pMDFile->szRawLocalDigest, &mdCTX);

	if (ferror(fLocalFile))
	{
		fclose(fLocalFile);
		return false;
	}

	// use following output to retreive md5 uint32 array's in md5sums.html
	/*char path[255];
	sprintf(path, "%sSAMP/md5sums.html", g_pszStorage);
	FILE* md5sums = fopen(path, "a");
	fprintf(md5sums, "[%s]: mdCTX UINT32 buf[]: %u, %u, %u, %u<br>\n", pMDFile->szFileLocation, mdCTX.buf[0], mdCTX.buf[1], mdCTX.buf[2], mdCTX.buf[3]);
	fprintf(md5sums, "[%s]: mdChkFile->iCorrectDigestArray[]: %u %u %u %u<br>\n", pMDFile->szFileLocation, pMDFile->iCorrectDigestArray[0], pMDFile->iCorrectDigestArray[1], pMDFile->iCorrectDigestArray[2], pMDFile->iCorrectDigestArray[3]);
	fclose(md5sums);*/

	if ((pMDFile->iCorrectDigestArray[0] != mdCTX.buf[0]) || (pMDFile->iCorrectDigestArray[1] != mdCTX.buf[1]) || (pMDFile->iCorrectDigestArray[2] != mdCTX.buf[2]) || (pMDFile->iCorrectDigestArray[3] != mdCTX.buf[3]))
	{
		//Log(encError.decrypt(), pMDFile->szFileLocation);
		fclose(fLocalFile);
		return false;
	}

	fclose(fLocalFile);
	return true;
}

#include "cryptors/CHECK_HASH2_result.h"

bool CCheckFileHash::IsFilesValid()
{
	bool bReturn = true;

	SMDFile* mdCheckSumFile = new SMDFile;

	for (int i = 0; i < MAX_HASHED_FILES; i++)
	{
		sprintf(mdCheckSumFile->szFileLocation, g_aHashedFiles[i].encStr.decrypt(), g_pszStorage);

		mdCheckSumFile->iCorrectDigestArray[0] = g_aHashedFiles[i].uiCorrectDigest1;
		mdCheckSumFile->iCorrectDigestArray[1] = g_aHashedFiles[i].uiCorrectDigest2;
		mdCheckSumFile->iCorrectDigestArray[2] = g_aHashedFiles[i].uiCorrectDigest3;
		mdCheckSumFile->iCorrectDigestArray[3] = g_aHashedFiles[i].uiCorrectDigest4;

		if (!IsFileValid(mdCheckSumFile))
		{
			bReturn = false;
		}
	}

	delete mdCheckSumFile;

	return bReturn;
}
