#pragma once
#include <math.h>

class Vector {
public:
	Vector();
	Vector(float x, float y, float z);

	float length();	
	void normalize();

	float x;
	float y;
	float z;
};