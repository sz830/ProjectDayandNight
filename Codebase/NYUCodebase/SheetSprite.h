#pragma once
#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"
#include "SDL\SDL_image.h"
#include "Vector.h"

/*	The SheetSprite class is used to manage textures for objects.
*/
class SheetSprite {
public:
	SheetSprite();
	SheetSprite(GLuint textureID, float u, float v, float width, float height);

	void Draw(Vector position, Vector scale, float rotation, bool flipped);

	GLuint textureID;

	float u;
	float v;
	float width;
	float height;
};
