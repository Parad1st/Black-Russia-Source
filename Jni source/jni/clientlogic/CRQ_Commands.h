#pragma once

#include "../main.h"

class CRQ_Commands {
public:
	static void rqVertexBufferSelect_HOOK(unsigned int **result);
	static void rqVertexBufferDelete_HOOK(int result);
};