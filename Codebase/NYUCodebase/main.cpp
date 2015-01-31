extern int viewPortX = 1000;
extern int viewPortY = 800;
extern float yTox = (float)viewPortY / (float)viewPortX;
extern int LEVEL_WIDTH = 20;
extern int LEVEL_HEIGHT = LEVEL_WIDTH*yTox;
extern float TILE_SIZE = 2.f / (float)LEVEL_WIDTH;
extern float WORLD_OFFSET_X = 1.;
extern float WORLD_OFFSET_Y = yTox;

void worldToTileCoordinates(float worldX, float worldY, int *gridX, int *gridY) {
	*gridX = (int)((worldX + (WORLD_OFFSET_X)) / TILE_SIZE);
	*gridY = (int)((-worldY + (WORLD_OFFSET_Y)) / TILE_SIZE);
}
float gridToYTop(int x, int y)
{
	float movedown = TILE_SIZE*y;
	return WORLD_OFFSET_Y - movedown;
}
float gridToYBot(int x, int y)
{
	float movedown = TILE_SIZE*(y + 1);
	return WORLD_OFFSET_Y - movedown;
}
float gridToXLeft(int x, int y)
{
	float moveright = TILE_SIZE*(x);
	return -WORLD_OFFSET_X + moveright;
}
float gridToXRight(int x, int y)
{
	float moveright = TILE_SIZE*(x + 1);
	return -WORLD_OFFSET_X + moveright;
}

#pragma once
#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"
#include "SDL\SDL_image.h"
#include "SDL\SDL_mixer.h"
#include <vector>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cmath>
#include <string>
#include <time.h>
#include "Vector.h"
#include "Player.h"
#include "NPC.h"
#include "Entity.h"
#include "World.h"
#include "Engine.h"
#include "AppDelegate.h"

/*
Template for Project Day and Night
Started 1/6/15 by Simon Zhang
an attempt to create a working multi-level two-part game engine

The engines are RPG style, and Platformer Style.
A third mini engine - textEngine will be responsible for dialogue boxes and forwarding their text

Levels will be kept track of outside the engine
Dialogue will be set using the engine
Reading world data from files is important
Entering room, starting new level, starting game, leaving room, switching between day and night sequence is a potential engine switching action
Context switching may happen here.
Each one will act as reinitializing the engine Engine

From hereon RPGEngine will be referred to DayEngine. PlatformerEngine will be referred to as NightEngine.

Outside Engine
include Player.h
include NPC.h
bool dayTime; //decides which engine to use
Textures
music
world Obj data
dialogue data
level - 1=day1,2=night1,3=day2,4=night2...

DayEngineRequirements

Player player;
vector<NPC*> NPCs;
vector<Entity*> entities; //Objects that can be interacted with or have depth issues

DayEngine decides when the NPC's at any given moment will move
for(all NPC's)
	if npc is NOT moving
		1% chance he will begin executing a move
			npc.moveRandom()

Player movement

Player.moveRight//Left//Up//Down

Render(); // render everything it should
	First draw the grid it's using
	employ depth and alpha testing
	next draw Entities
	next draw NPC's
	next draw Player
	disable depth and alpha testing


*/
using namespace std;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	AppDelegate app = AppDelegate();
	while (!app.UpdateAndRender())
	{ }
	return 0;
}