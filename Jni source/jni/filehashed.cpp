#include "filehashed.h"

#include "game/common.h"

#ifdef GAME_EDITION_CR

const CFileHashed g_aHashedFiles[MAX_HASHED_FILES] = {
	CFileHashed("%sSAMP/main.scm", 15, 0, 1383077426, 2907948810, 1728064869),
	CFileHashed("%sSAMP/script.img", 17, 0, 3878717776, 708762158, 97013146),
	CFileHashed("%sSAMP/handling.cfg", 19, 0, 461480902, 1005661073, 3884439221),
	CFileHashed("%sSAMP/weapon.dat", 17, 0, 3929418255, 399917113, 2550983597)
};

#else

const CFileHashed g_aHashedFiles[MAX_HASHED_FILES] = {
	CFileHashed("%sSAMP/main.scm", 15, 0, 3212378963, 3226940476, 2512197760),
	CFileHashed("%sSAMP/script.img", 17, 0, 2761982703, 3892831165, 2244854639),
	CFileHashed("%sSAMP/handling.cfg", 19, 0, 1642125591, 3667380089, 2589095260),
	CFileHashed("%sSAMP/weapon.dat", 17, 0, 1225869809, 3137529211, 3241731015)
};

#endif