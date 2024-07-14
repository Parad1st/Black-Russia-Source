#include "CVector.h"

CVector::CVector()
{
	vector.X = 0.0f;
	vector.Y = 0.0f;
	vector.Z = 0.0f;
}

CVector::CVector(float value)
{
	vector.X = value;
	vector.Y = value;
	vector.Z = value;
}

CVector::CVector(float x, float y, float z)
{
	vector.X = x;
	vector.Y = y;
	vector.Z = z;
}

VECTOR CVector::Set(float value)
{
	vector.X = value;
	vector.Y = value;
	vector.Z = value;

	return vector;
}

VECTOR CVector::Set(float x, float y, float z)
{
	vector.X = x;
	vector.Y = y;
	vector.Z = z;

	return vector;
}

VECTOR CVector::Get()
{
	return vector;
}

VECTOR CVector::Zero()
{
	vector.X = 0.0f;
	vector.Y = 0.0f;
	vector.Z = 0.0f;

	return vector;
}
