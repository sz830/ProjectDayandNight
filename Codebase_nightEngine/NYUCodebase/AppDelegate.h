#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

GLuint LoadTexture(const char *image_path) {
	SDL_Surface *surface = IMG_Load(image_path);
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	SDL_FreeSurface(surface);
	return textureID;
}

class AppDelegate {
	public:
		bool done;
		float lastTicks;
		SDL_Window* displayWindow;
		SDL_Event event;
		int level; //0=day 1

		Player * player;

		//Worlds
		World * testWorld;

		//Textures
		GLuint terrainID;
		GLuint playerID;
		GLuint fontID;

		//Engine
		Engine *e;

		AppDelegate();
		~AppDelegate();

		void Init();
		bool UpdateAndRender();
		void Update(float elapsed);
		void Render();

		void InitLevel1();
		void UpdateLevel1(float elapsed);
};

AppDelegate::AppDelegate(){
	Init();
	done = false;
	lastTicks = (float)SDL_GetTicks() / 1000.0f;;
}
AppDelegate::~AppDelegate() {
	SDL_Quit();
}

bool AppDelegate::UpdateAndRender(){
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastTicks;
	lastTicks = ticks;
	Update(elapsed);
	Render();
	return done;
}
void AppDelegate::Init(){ // Load all textures and Worlds
	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("Project Day and Night", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, viewPortX, viewPortY, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	glClearColor(0.3f, .3f, .3f, 1.0f);

	//Setup
	glViewport(0, 0, viewPortX, viewPortY);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1., 1., -yTox, yTox, -10.0, 10.0);
	glMatrixMode(GL_MODELVIEW);

	//Textures
	terrainID = LoadTexture("pngs/testTerrain.png");
	playerID = LoadTexture("pngs/madotsuki.png");
	fontID = LoadTexture("pngs/font.png");

	//Player
	player = new Player(5, 5, playerID, "Mary");

	//Worlds - Some people call these maps
	vector<int> solids = { 7, 8, 9, 13, 14, 18, 20, 22, 31, 33, 34, 35, 36, 61, 62, 65, 66, 67, 78, 80, 91, 92, 93 };
	testWorld = new World("maps/testWorld.txt", 13, 8, terrainID, solids);
	
	//Engine
	e = new Engine(fontID);
	
	InitLevel1();
}
void AppDelegate::Update(float elapsed){
	switch (level) {
	case 1:
		UpdateLevel1(elapsed);
		break;
	}
}

void AppDelegate::Render(){
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(-player->position->x,-player->position->y,0); // Camera follows Player
	
	e->Render();

	glDepthMask(GL_FALSE);
	SDL_GL_SwapWindow(displayWindow);
}

void AppDelegate::InitLevel1(){
	level = 1;
	e = new Engine();
	e->world = testWorld;
	
	//Modify and Add Player
	e->player = player;
	player->width / 2.f + .0001;

	//Create and add NPC's
	NPC *npc1 = new NPC(7, 7, playerID, "Luke"); // uses playerID because there is no npcID yet
	e->addNPC(npc1);
	NPC *npc2 = new NPC(7, 12, playerID, "Johnny"); // uses playerID because there is no npcID yet
	e->addNPC(npc2);
	NPC *npc3 = new NPC(12, 7, playerID, "Ron"); // uses playerID because there is no npcID yet
	e->addNPC(npc3);
	NPC *npc4 = new NPC(12, 12, playerID, "Abe"); // uses playerID because there is no npcID yet
	e->addNPC(npc4);

	//Create and add Entities
}

void AppDelegate::UpdateLevel1(float elapsed){
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_KEYDOWN){
			if (event.key.keysym.scancode == SDL_SCANCODE_Z) {
				//Player interact action
			}
		}
	}
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	//Horizontal movement
	if (keys[SDL_SCANCODE_RIGHT]) {
		e->moveRight(elapsed);
	}
	else if (keys[SDL_SCANCODE_LEFT]) {
		e->moveLeft(elapsed);
	}
	else {
		e->stopPlayerHorizontal();
	}

	//Vertical Movement
	if (keys[SDL_SCANCODE_DOWN]) {
		e->moveDown(elapsed);
	}
	else if (keys[SDL_SCANCODE_UP]) {
		e->moveUp(elapsed);
	}
	else {
		//e->stopPlayerVertical();
	}

	e->Update(elapsed);
}