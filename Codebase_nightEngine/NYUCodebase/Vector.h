#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

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