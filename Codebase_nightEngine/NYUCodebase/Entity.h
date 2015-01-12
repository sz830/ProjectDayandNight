/*
Objects that can be interacted with or have depth issues (like tall trees or gates).
These will always be collidable objects

float [] collision boundry - has to be in terms of grid coordinates. to prevent awkward approximation errors
dialogue or interaction parameters
	

Render
	normal rendering but using depth and alpha testing

*/
#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity{
public:
	Entity();
	~Entity();
	std::vector<Vector*> collisionLocations; // array of all the grid coordinates that this entity is collidable at

	void Draw();
};

Entity::~Entity(){}
Entity::Entity(){}

void Entity::Draw()
{

}