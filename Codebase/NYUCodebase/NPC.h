/*
NPC's information

Player and NPC will have
string name;
bool moving; //If the player or NPC is moving, it ignores movement input.
ENUM facing; //north east south west
bool interacting; //If the NPC is interacting he won't be able to move (ie. being talked to by Player)
int [] destination; // if moving, the grid coordinates of his destination. else, his current grid coordinates. So NPC's don't target the same destination and collide
Vector * velocity; // speed in X and Y
Vector * position; // position in X and Y
float speed = X;
Dialogue dialogue;
textureID

npc.init(grid coordinates, "dialogue.txt") // set up position, and all its other variables


npc.moveRandom() // npc picks random direction and attempts to move.
if(npc is not interacting)
	Pick a random direction and attempt to move in that direction
	update 'facing': NPC's sometimes attempt to walk into walls. They do not move but do change the direction they're facing.
	if(no collision block is in random direction)
		moving=true; velocity[0] or velicty[1] = + or - speed;

npc.update(float elapsed) //If Moving, continue in direction or finish moving(includes setting velocity back to 0)
	if(moving)
		depending on 'facing', (position.x or position.y) += elapsed* (velocity.x or velocity.y)
	if(npc has moved past his destination in x or y axis)
		npc.position = destination- converted to position coordinates;
		moving=false;
		velocity=0;


*/
#pragma once
#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"
#include "SDL\SDL_image.h"

using namespace std;

//Animation Information
vector<int> upNPCAnimation = { 9, 10, 11, 10 };
vector<int> rightNPCAnimation = { 21, 22, 23, 22 };
vector<int> downNPCAnimation = { 33, 34, 35, 34 };
vector<int> leftNPCAnimation = { 45, 46, 47, 46 };
int upNPCStanding = 10;
int rightNPCStanding = 22;
int downNPCStanding = 34;
int leftNPCStanding = 46;


class NPC{
public:
	NPC();
	~NPC();
	NPC(int gridX, int gridY, GLuint textureID, const char *name);

	const char *name;
	Vector*position;
	Vector*velocity;
	Vector*destination;
	float speed = .5;
	bool moving = false;
	int facing; // 0=north 1=east 2=south 3=west
	bool interacting = false;
	GLuint textureID;
	float movementTime = 0.f;
	//Dialogue Data Structure

	//RenderingInformation
	float width = TILE_SIZE;
	float height = 1.33*TILE_SIZE;
	int spriteCountX = 12;
	int spriteCountY = 8;

	//Rendering Animation Information
	float animationElapsed = 0.0;
	int animationIndex = 0;
	int lastFacing = -1; // direction player was last facing - read from input
	float fps = 7.0; // animation frames per second
	bool movementInput = false; // whether or not 

	void updateAnimationInfo(int facing, float elapsed);

	void Draw();
	void DrawSpriteSheetSprite(int index);
};

NPC::~NPC(){}
NPC::NPC(){}
NPC::NPC(int gridX, int gridY, GLuint textureID, const char *name){
	NPC::name = name;
	NPC::destination = new Vector(gridX, gridY, 0);
	NPC::position = new Vector(gridToXLeft(gridX, gridY) + TILE_SIZE / 2.f, gridToYTop(gridX, gridY) - TILE_SIZE / 2.f, 0.0);
	NPC::velocity = new Vector(0, 0, 0);
	NPC::textureID = textureID;
	NPC::facing = 2;
}

void NPC::Draw()//Not ready yet because player is still moving too fast
{
	if (!moving && movementInput == false){
		if (facing == 0) { DrawSpriteSheetSprite(upNPCStanding); } //NORTH
		else if (facing == 1) { DrawSpriteSheetSprite(rightNPCStanding); } //EAST
		else if (facing == 2) { DrawSpriteSheetSprite(downNPCStanding); } //SOUTH
		else if (facing == 3) { DrawSpriteSheetSprite(leftNPCStanding); } //WEST
	}
	else {
		if (facing == 0){//NORTH
			DrawSpriteSheetSprite(upNPCAnimation[animationIndex%upNPCAnimation.size()]);
		}
		else if (facing == 1){//EAST
			DrawSpriteSheetSprite(rightNPCAnimation[animationIndex%rightNPCAnimation.size()]);
		}
		else if (facing == 2){//SOUTH
			DrawSpriteSheetSprite(downNPCAnimation[animationIndex%downNPCAnimation.size()]);
		}
		else if (facing == 3){//WEST
			DrawSpriteSheetSprite(leftNPCAnimation[animationIndex%leftNPCAnimation.size()]);
		}
	}
}

void NPC::DrawSpriteSheetSprite(int index) {
	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID); // so gltexture_2d knows what textureid to draw
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glTranslatef(position->x, position->y, 0.0);

	float depth = -(position->y - (-LEVEL_HEIGHT*TILE_SIZE)) / (2 * LEVEL_HEIGHT*TILE_SIZE);
	GLfloat vertices[] = { -width / 2.f, height - width / 2.f, depth,
		-width / 2.f, -width / 2.f, depth,
		width / 2.f, -width / 2.f, depth,
		width / 2.f, height - width / 2.f, depth
	};
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	float u = (float)(((int)index) % spriteCountX) / (float)spriteCountX;
	float v = (float)(((int)index) / spriteCountX) / (float)spriteCountY;
	float spriteWidth = 1.0f / (float)spriteCountX;
	float spriteHeight = 1.0f / (float)spriteCountY;

	GLfloat quadUVs[] = { u, v,
		u, v + spriteHeight,
		u + spriteWidth, v + spriteHeight,
		u + spriteWidth, v
	};

	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);//enables blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDrawArrays(GL_QUADS, 0, 4);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_ALPHA_TEST);
	glPopMatrix();
}

void NPC::updateAnimationInfo(int facing, float elapsed){
	if (facing == lastFacing){
		animationElapsed += elapsed;
		if (animationElapsed > 1.0 / fps) {
			animationIndex++;
			animationElapsed = 0.0;
		}
	}
	else {
		lastFacing = facing;
		animationElapsed = 0.0;
		animationIndex = 0;
	}
}