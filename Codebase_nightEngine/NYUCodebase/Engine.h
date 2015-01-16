#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

class Engine{
public:
	Engine();
	Engine(GLuint fontID);
	~Engine();
	Player *player;
	vector<NPC*> NPCs;
	vector<Entity*> entities; //Objects that can be interacted with or have depth issues
	World * world;
	bool dayTime;
	const float smallWorldUnit = .1f; //Used for collision detection. Not actually sure how to come up with a suitably small number.
	bool collisionsEnabled = false; //Enables/disables collision detection. Meant for debugging.

	void Render();
	void RenderWorld();

/*	void moveUp();
	void moveDown();*/
	void moveLeft();
	void moveRight();
	void stopPlayerHorizontal();
	void stopPlayerVertical();
	void stopPlayer();
	bool isCollision(int gridX, int gridY);

	void Update(float elapsed);
	void UpdatePlayer(float elapsed);
	void UpdateNPCs(float elapsed);

	void addNPC(NPC* npc);
	void addEntity(Entity* entity);

	//Displays dialogue, renderDialogue("hello world!",0.5,0.5); // HEINOUS - Emily's working on this
	void renderDialogue(string text, float x, float y);
	GLuint fontID;
};

Engine::~Engine(){}
Engine::Engine(){}
Engine::Engine(GLuint fontID){
	Engine::fontID = fontID;
}

void Engine::Render()
{
	//Draw world
	RenderWorld();

	//Draw Entities
	for (int i = 0; i < entities.size(); i++){
		entities[i]->Draw();
	}
	//Draw Player
	player->Draw();

	//Draw NPC's
	for (int i = 0; i < NPCs.size(); i++){
		NPCs[i]->Draw();
	}
}

void Engine::RenderWorld(){
	vector<float> vertexData;
	vector<float> texCoordData;
	int size = 0;
	for (int y = 0; y < world->mapHeight; y++)
	{
		for (int x = 0; x < world->mapWidth; x++)
		{
			if (world->levelData[y][x] != 0)
			{
				float u = (float)(((int)world->levelData[y][x]) % world->spriteCountX) / (float)world->spriteCountX;
				float v = (float)(((int)world->levelData[y][x]) / world->spriteCountX) / (float)world->spriteCountY;
				float spriteWidth = 1.0f / (float)world->spriteCountX;
				float spriteHeight = 1.0f / (float)world->spriteCountY;
				vertexData.insert(vertexData.end(), {
					TILE_SIZE * x, -TILE_SIZE * y,
					TILE_SIZE * x, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, (-TILE_SIZE * y) - TILE_SIZE,
					(TILE_SIZE * x) + TILE_SIZE, -TILE_SIZE * y
				});
				texCoordData.insert(texCoordData.end(), { u, v,
					u, v + (spriteHeight),
					u + spriteWidth, v + (spriteHeight),
					u + spriteWidth, v
				});
				size += 4;
			}
		}
	}
	//Set up
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glBindTexture(GL_TEXTURE_2D, world->textureID); // so gltexture_2d knows what textureid to draw
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(-1.f, yTox, 0.f);

	//Draw
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glDrawArrays(GL_QUADS, 0, size);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisable(GL_BLEND);
	glPopMatrix();
}
/*void Engine::moveUp(){
	if (!player->moving){
		player->facing = 2;//NORTH
		//Check that the grid block to the north is collision free 
		int y = (int)player->destination->y - 1;
		int x = (int)player->destination->x;
		if (!isCollision(x, y)){
			player->moving = true;
			player->velocity->y = player->speed;
			player->destination->y = y;
		}
	}
}
void Engine::moveDown(){\
		player->facing = 3;//SOUTH
		//Check that the grid block to the south is collision free 
		int y = (int)player->destination->y + 1;
		int x = (int)player->destination->x;
		if (!isCollision(x, y)){
			player->moving = true;
			player->velocity->y = -player->speed;
			player->destination->y = y;
		}\
}*/
void Engine::moveLeft(){
//	if (!player->moving){
		player->facing = 0;//WEST
		//Check that the grid block to the west is collision free 
		int y = (int)player->destination->y;
		int x = (int)player->destination->x - smallWorldUnit;
		if (!isCollision(x, y)){
/*			string debug_string = "Move left: no collision\n"; //DEBUG
			OutputDebugString(debug_string.data()); //DEBUG*/
			player->moving = true;
			player->velocity->x = -player->speed;
			string debug_string = "player->velocity->x: " + to_string(player->velocity->x) + "\n"; //DEBUG
			OutputDebugString(debug_string.data()); //DEBUG
//			player->destination->x = x;
		}
//	}
}
void Engine::moveRight(){
//	if (!player->moving){
		player->facing = 1;//EAST
		//Check that the grid block to the east is collision free 
		int y = (int)player->position->y;
		int x = (int)player->position->x + smallWorldUnit;
		if (!isCollision(x, y)){
/*			string debug_string = "Move right: no collision\n"; //DEBUG
			OutputDebugString(debug_string.data()); //DEBUG*/
			player->moving = true;
			player->velocity->x = +player->speed;
/*			string debug_string = "player->destination->x: " + to_string(player->destination->x) + "\n"; //DEBUG
			OutputDebugString(debug_string.data()); //DEBUG*/
		}
//	}
}
void Engine::stopPlayerHorizontal(){
	player->velocity->x = 0;
	player->animationTime = 0;
	player->moving = false; //TEMPORARY
}
void Engine::stopPlayerVertical() {
	player->velocity->y = 0;
}
void Engine::stopPlayer() {
	stopPlayerHorizontal();
	stopPlayerVertical();
}

bool Engine::isCollision(int gridX, int gridY){
	if (!collisionsEnabled) {
		return false;
	}

	if (gridY >= 0 && gridY < world->mapHeight && gridX >= 0 && gridX<world->mapWidth){ //Check if coordinates are out of bounds
		for (int i = 0; i < world->solids.size(); i++){ // check map if new destination is a collision tile
			if (world->levelData[gridY][gridX] == world->solids[i]){
				return true;
			}
		}
		for (int i = 0; i < entities.size(); i++){ // for every entity
			//check for collision between entity and the grid that player is headed towards
			for (int j = 0; j < entities[i]->collisionLocations.size(); j++){ // for each collisionLocation
				if ((int)entities[i]->collisionLocations[j]->x == gridX){
					if ((int)entities[i]->collisionLocations[j]->y == gridY){
						return true;
					}
				}
			}
		}
		for (int i = 0; i < NPCs.size(); i++){ //check for collision between npc's 'destination' and the new destination your player is headed towards
			if ((int)NPCs[i]->destination->x == gridX){
				if ((int)NPCs[i]->destination->y == gridY){
					return true;
				}
			}
		}
		if ((int)player->destination->x == gridX){ // check for collision with player's current 'destination' // shoudln't be a problem
			if ((int)player->destination->y == gridY){
				return true;
			}
		}
	}
	else {
		return true;
	}
	return false;
}

void Engine::Update(float elapsed){
	//Update Player
	UpdatePlayer(elapsed);
	//Update NPCs
	UpdateNPCs(elapsed);
/*	string s = to_string(player->velocity->x) + "\n"; //DEBUG
	OutputDebugString(s.data()); //DEBUG*/
}
void Engine::UpdatePlayer(float elapsed){
	if (player->moving){
		player->position->x += player->velocity->x*elapsed;
		player->position->y += player->velocity->y*elapsed;
		player->animationTime += elapsed;
	}
	//Player reaches destination when animationTime > distance/velocity ; aka, we know how much time it takes him to travel one grid location
	if (player->animationTime >= TILE_SIZE / player->speed){ 
		//int gridX = player->destination->x;
		//int gridY = player->destination->y;
		player->animationTime = 0;
		//player->moving = false;
		//player->position = new Vector(gridToXLeft(gridX, gridY) + TILE_SIZE / 2.f, gridToYTop(gridX, gridY) - TILE_SIZE / 2.f, 0.0);
		//player->velocity = new Vector(0, 0, 0);
	}
}

void Engine::UpdateNPCs(float elapsed){
	//Commented out to make the NPCs immobile for now
/*	for (int i = 0; i < NPCs.size(); i++){
		NPC * npc = NPCs[i];
		if (!npc->moving){
			int prob = rand() % 1000;
			if (prob < 5){ //1% chance npc begins to move when not moving
				int dir = (rand() % 4);
				npc->facing = dir;
				//Check that the grid block to the north is collision free 
				int x, y;
				if (dir == 0){//north
					y = (int)npc->destination->y - 1;
					x = (int)npc->destination->x;
				}
				else if (dir == 1){//east
					y = (int)npc->destination->y;
					x = (int)npc->destination->x+1;
				}
				else if (dir == 2){//south
					y = (int)npc->destination->y + 1;
					x = (int)npc->destination->x;
				}
				else if (dir == 3){//west
					y = (int)npc->destination->y;
					x = (int)npc->destination->x - 1;
				}
				
				bool collision = isCollision(x, y);
				if (!collision){
					npc->moving = true;
					if (dir == 0){ npc->velocity->y = npc->speed; }
					else if (dir == 1){ npc->velocity->x = npc->speed; }
					else if (dir == 2){ npc->velocity->y = -npc->speed; }
					else if (dir == 3){ npc->velocity->x = -npc->speed; }
					npc->destination->y = y;
					npc->destination->x = x;
				}
			}
		}
		else {
			npc->position->x += npc->velocity->x*elapsed;
			npc->position->y += npc->velocity->y*elapsed;
			npc->animationTime += elapsed;

			//npc reaches destination when animationTime > distance/velocity ; aka, we know how much time it takes him to travel one grid location
			if (npc->animationTime >= TILE_SIZE / npc->speed){
				int gridX = npc->destination->x;
				int gridY = npc->destination->y;
				npc->animationTime = 0;
				npc->moving = false;
				npc->position = new Vector(gridToXLeft(gridX, gridY) + TILE_SIZE / 2.f, gridToYTop(gridX, gridY) - TILE_SIZE / 2.f, 0.0);
				npc->velocity = new Vector(0, 0, 0);
			}
		}
	}*/
}

void Engine::addNPC(NPC* npc){
	NPCs.push_back(npc);
}
void Engine::addEntity(Entity* entity){
	entities.push_back(entity);
}

void Engine::renderDialogue(string text, float x, float y){//Displays dialogue, renderDialogue("hello world!",0.5,0.5); // HEINOUS - Emily's working on this
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glBindTexture(GL_TEXTURE_2D, fontID); // so gltexture_2d knows what textureid to draw
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(x, y, 0);
	float texture_size = 1.0 / 16.0f;

	float r = .5, g = .5, b = .5, a = 1.0;
	float spacing = 0.;
	float size = .05f;

	vector<float> vertexData;
	vector<float> texCoordData;
	vector<float> colorData;

	for (unsigned int i = 0; i < text.size(); i++)
	{
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });

		vertexData.insert(vertexData.end(), { ((size + spacing)*i) + (-0.5f*size), .5f*size,
			((size + spacing)*i) + (-.5f*size), -.5f*size,
			((size + spacing)*i) + (.5f*size), -.5f*size,
			((size + spacing)*i) + (.5f*size), .5f*size
		});

		texCoordData.insert(texCoordData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x +
			texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, 0, texCoordData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glDrawArrays(GL_QUADS, 0, text.size() * 4);

	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisable(GL_BLEND);
	glPopMatrix();
}

