#pragma once

#include "../main.h"
#include "common.h"

class CVector {
public:
	CVector();
	CVector(float value);
	CVector(float x, float y, float z);

	VECTOR Set(float value);
	VECTOR Set(float x, float y, float z);

	VECTOR Get();

	VECTOR Zero();

private:
	VECTOR vector;
};