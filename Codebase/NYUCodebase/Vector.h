#pragma once
#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"
#include "SDL\SDL_image.h"

class Vector {
public:
	Vector();
	Vector(float x, float y, float z);
	~Vector();

	float x;
	float y;
	float z;
};
Vector::Vector(){}
Vector::Vector(float x, float y, float z){
	Vector::x = x;
	Vector::y = y;
	Vector::z = z;
}
Vector::~Vector(){}