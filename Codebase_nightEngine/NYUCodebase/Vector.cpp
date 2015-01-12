#include "Vector.h"

Vector::Vector() : x(0.0f), y(0.0f), z(0.0f) {}

Vector::Vector(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector::length() {
	return sqrt( x * x + y * y + z * z);
}

void Vector::normalize() {
	float theLength = length();
	x = x / theLength;
	y = y / theLength;
	z = z / theLength;
}