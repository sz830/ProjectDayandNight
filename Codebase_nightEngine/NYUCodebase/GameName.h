#pragma once
#include <SDL_mixer.h>
#include <string>
#include <stdio.h>
#include <string>
#include <iostream>
#include <iomanip>
#include "Entity.h"
#include "Textures.h"

using namespace std;

#define FIXED_TIMESTEP 0.0166666f			// 60 FPS
//#define FIXED_TIMESTEP 0.0333333f			// 30 FPS
#define MAX_TIMESTEPS 6

class GameName {
public:
	GameName();
	~GameName();

	//	Basic Functions
	void Init();
	void Render();
	void Update(float elapsed);
	void FixedUpdate();
	bool UpdateAndRender();

	//	Function for printing text to the screen
	void Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a);

	//	Lerp Function
	float lerp(float v0, float v1, float t);

	//======================================================================================================================
	//--- NEW FUNCTIONS FOR THE GAME GOES HERE
	//======================================================================================================================

private:
	bool done = false;
	float lastFrameTicks = 0.0f;
	float timeLeftOver = 0.0f;
	SDL_Window* displayWindow;

	//	Gravity
	Vector gravity;

	//	Mouse Motion && Screen Resolution :
	Vector ScreenResolution;
	float aspectRatio;
	Vector mousePosition;

	//	Textures :
	Textures* textures;

	//	Music & Sounds :			Mix_Music* musicName;	Mix_Chunk* soundName;

	//	Entities :					Entity* entityName;		vector<Entity*> entites;

	//	Game states :				enum GameState { STATE_MAINMENU, etc };		int gameState = STATE_MAINMENU;		
};