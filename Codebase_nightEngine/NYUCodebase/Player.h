/*
Player's information
player and NPC will have
bool moving; //If the player or NPC is moving, it ignores movement input.
gridCoordinates
Vector position
textureID
Inventory - will be held by player

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


*/

#pragma once
#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

using namespace std;

//Animation Information
vector<int> rightPlayerAnimation = { 12, 13, 14, 13 };
vector<int> leftPlayerAnimation = { 36, 37, 38, 37 };
int rightPlayerStanding = 13;
int leftPlayerStanding = 37;


class Player{
public:
	Player();
	Player(int gridX, int gridY, GLuint textureID, const char *name);
	~Player();
	const char *name;
	Vector*position;
	Vector*velocity;
	Vector*destination;
	float speed = .5;
	bool movingX = false;
	bool movingY = false;
	int facing; // 0=north 1=east 2=south 3=west
	bool interacting=false;
	GLuint textureID;
	float animationTime = 0.f;

	//RenderingInformation
	float width=TILE_SIZE;
	float height=1.33*TILE_SIZE;
	int spriteCountX = 12;
	int spriteCountY = 8;

	void Draw();
	void DrawSpriteSheetSprite(int index);
};

Player::~Player(){}
Player::Player(int gridX, int gridY, GLuint textureID, const char *name){
	Player::name = name;
	Player::destination = new Vector(gridX, gridY, 0);
	Player::position = new Vector(gridToXLeft(gridX, gridY) + TILE_SIZE / 2.f, gridToYTop(gridX, gridY) - TILE_SIZE / 2.f, 0.0);
	Player::velocity = new Vector(0, 0, 0);
	Player::textureID = textureID;
	Player::facing = 1;
}
Player::Player(){}

void Player::Draw()
{
	float completionTime = TILE_SIZE / speed;
	int index = 0;
	if (!movingX){
			 if (facing == 0) { DrawSpriteSheetSprite(leftPlayerStanding); } //LEFT
		else if (facing == 1) { DrawSpriteSheetSprite(rightPlayerStanding); } //RIGHT
	}
	else {
		if (facing == 1) {//RIGHT
			float interval = completionTime / rightPlayerAnimation.size();
			for (float i = interval; i <= completionTime; i += interval){
				if (animationTime < i){
					DrawSpriteSheetSprite(rightPlayerAnimation[index%rightPlayerAnimation.size()]);
					break;
				}
				index++;
			}
		}
		else if (facing == 0) {//LEFT
			float interval = completionTime / leftPlayerAnimation.size();
			for (float i = interval; i <= completionTime; i += interval){
				if (animationTime < i){
					DrawSpriteSheetSprite(leftPlayerAnimation[index%leftPlayerAnimation.size()]);
					break;
				}
				index++;
			}
		}
	}
}

void Player::DrawSpriteSheetSprite(int index) {
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
	GLfloat vertices[] = {	-width / 2.f, height - width / 2.f, depth,
							-width / 2.f, -width / 2.f,			depth,
							width / 2.f, -width / 2.f,			depth,
							width / 2.f, height - width / 2.f,  depth
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