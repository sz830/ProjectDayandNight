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
	vector<Entity*> entities; // Objects that can be interacted with or have depth issues
	World * world;
	bool dayTime;
	#define smallWorldUnit .0001f; // Used for collision detection.
	bool collisionsEnabled = true; // Enables/disables collision detection. Meant for debugging.
	float gravity = -.5; // Gravity (constant acceleration) - set to 0 to disable, or positive for reverse-gravity!
	float term_vel = -1.f; // Terminal velocity (max. downward velocity) - smaller negative value INCREASES max downward speed
	bool flyEnabled = true; // Enables/disables moveUp()
	float fly_power = 1.3f; // "Strength" of moveUp() function. Directly counteracts (positive) gravity
	float fly_max = 1.5f; // Max upward speed achievable by using moveUp()

	void Render();
	void RenderWorld();

	// These functions modify player->velocity but not player->position. player->position is handled in Update()
	void moveUp(float elapsed); // Flying movement - can be used to stay in the air indefinitely. Mostly for debugging.
	void jump(float elapsed);
	void moveDown(float elapsed); //Mostly for debugging.
	void moveLeft(float elapsed);
	void moveRight(float elapsed);

	void stopPlayerHorizontal();
	void stopPlayerVertical();
	void stopPlayer(); // Invokes stopPlayerHorizontal() and stopPlayerVertical()

	int isCollision(float worldX, float worldY); // Takes world coordinates as parameters, NOT tile/grid coordinates
	int collisionKeyOr(int key1, int key2); // Helper function to interpret collisionKey values of two points. See isCollision() decription.
	void collisionCheckUp(); // Collision detection above
	void collisionCheckDown(); // Collision detection below
	void collisionCheckLeft(); // Collision detection to the left
	void collisionCheckRight(); // Collision detection to the right

	void Update(float elapsed);
	void UpdatePlayer(float elapsed);
	void UpdateNPCs(float elapsed);
	void applyGravity(float elapsed); // Applies gravity

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

void Engine::moveUp(float elapsed){
	if (flyEnabled) {
		//player->facing = 2;//NORTH
		if (player->velocity->y < fly_max) {
			player->velocity->y += fly_power * elapsed;
			if (player->velocity->y > fly_max) {
				player->velocity->y = fly_max;
			}
		}
			
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		//player->destination->x = x;
	}
}
void Engine::jump(float elapsed) {
	if (player->isOnPlatform) {
		player->velocity->y = +player->jumpSpeed;
	}
}
void Engine::moveDown(float elapsed){
	//player->facing = 3;//SOUTH
	if (player->velocity->y > -player->speed) { //Min downward speed
		player->velocity->y = -player->speed;
	}
	if (player->velocity->y > term_vel) { // Check if current velocity is larger than term_vel
		player->velocity->y += gravity * elapsed;
		if (player->velocity->y < term_vel) {
			player->velocity->y = term_vel;
		}
	}
	/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
	OutputDebugString(debug_string.data()); //DEBUG*/
	//player->destination->x = x;
}
void Engine::moveLeft(float elapsed){
	player->facing = 0;//WEST
	player->velocity->x = -player->speed;
	/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
	OutputDebugString(debug_string.data()); //DEBUG*/
	//player->destination->x = x;
}
void Engine::moveRight(float elapsed){
	player->facing = 1;//EAST
	player->velocity->x = +player->speed;
	/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
	OutputDebugString(debug_string.data()); //DEBUG*/
	//player->destination->x = x;
}
void Engine::stopPlayerHorizontal(){
	player->velocity->x = 0;
	player->animationTime = 0;
	player->movingX = false;
}
void Engine::stopPlayerVertical() {
	player->velocity->y = 0;
	//player->animationTime = 0;
	player->movingY = false;
}
void Engine::stopPlayer() {
	stopPlayerHorizontal();
	stopPlayerVertical();
}

/*In order of precedence, this function returns...
	-1 if collisions are disabled
	0  if there is no collision
	1  if the coordinates are out-of-bounds
	2  if colliding with the world (solids)
	3  if colliding with entities
	4  if colliding with NPCs
	5  if colliding with the Player

	Return codes 4 and 5 are currently never returned
*/
int Engine::isCollision(float worldX, float worldY){
	if (!collisionsEnabled) {
		return -1;
	}

	int gridX, gridY;
	worldToTileCoordinates(worldX, worldY, &gridX, &gridY);

	if (gridY >= 0 && gridY < world->mapHeight && gridX >= 0 && gridX<world->mapWidth){ //Check if coordinates are out of bounds
		//Presumably, world solids will only ever take up integer units of tiles, so this can be left as-is.
		for (int i = 0; i < world->solids.size(); i++){ // check map if new destination is a collision tile
			if (world->levelData[gridY][gridX] == world->solids[i]){
				/*string debug_string = "worldX: " + to_string(worldX) + "\n"; //DEBUG
				OutputDebugString(debug_string.data()); //DEBUG
				debug_string = "worldY: " + to_string(worldY) + "\n"; //DEBUG
				OutputDebugString(debug_string.data()); //DEBUG*/
				return 2;
			}
		}
		//TODO May need to convert grid coordinates to world coordinates
		for (int i = 0; i < entities.size(); i++){ // for every entity
			//check for collision between entity and the grid that player is headed towards
			for (int j = 0; j < entities[i]->collisionLocations.size(); j++){ // for each collisionLocation
				if ((int)entities[i]->collisionLocations[j]->x == gridX){
					if ((int)entities[i]->collisionLocations[j]->y == gridY){
						return 3;
					}
				}
			}
		}
		/*for (int i = 0; i < NPCs.size(); i++){ //check for collision between npc's 'destination' and the new destination your player is headed towards
			if ((int)NPCs[i]->position->x == worldX){
				if ((int)NPCs[i]->position->y == worldY){
					return 4;
				}
			}
		}
		if ((int)player->position->x == worldX){ // check for collision with player's current 'destination' // shoudln't be a problem
			if ((int)player->position->y == worldY){
				return 5;
			}
		}*/
	}
	else {
		return 1;
	}
	return 0;
}
// Returns a collision key identical to those specified above isCollision()
// TODO Change priority based on further collision detection implementation
int Engine::collisionKeyOr(int key1, int key2) {
	int returnKey = 0;
	if (key1 == 1 || key2 == 1) {
		returnKey = 1;
	}
	if (key1 == 2 || key2 == 2) {
		returnKey = 2;
	}
	else if (key1 == -1 || key2 == -1) {
		returnKey = -1;
	}
	/*string s = to_string(returnKey) + "\n"; //DEBUG
	OutputDebugString(s.data()); //DEBUG*/
	return returnKey;
}
void Engine::collisionCheckUp() {
	//player->facing = 2;//NORTH
	//Check that the two grid blocks to the north are collision-free 
	float y = player->position->y + (player->height / 2.f);
	float x1 = player->position->x - (player->width / 2.f) + smallWorldUnit;
	int collisionKey1 = isCollision(x1, y);
	float x2 = player->position->x + (player->width / 2.f) - smallWorldUnit;
	int collisionKey2 = isCollision(x2, y);
	int collisionKey = collisionKeyOr(collisionKey1, collisionKey2);
	if (collisionKey <= 0){ //No collision OR collisions disabled
		/*string debug_string = "Move right: no collision\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		player->movingY = true;
		player->isOnPlatform = false; // Player is not standing on a platform
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		//player->destination->x = x;
	}
	//TODO Change based on level design/mechanics
	// Currently places the player back on the map
	else if (collisionKey == 1) { // Out-of-bounds
		stopPlayerVertical();
		int gridX, gridY;
		worldToTileCoordinates(x1, y, &gridX, &gridY);
		player->position->y = gridToYBot(gridX, gridY) - player->height / 2.f;
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
	}
	else if (collisionKey == 2) { // World (solids) collisions
		stopPlayerVertical();
		int gridX, gridY;
		worldToTileCoordinates(x1, y, &gridX, &gridY);
		player->position->y = gridToYBot(gridX, gridY) - player->height / 2.f; // fix penetration
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
	}
	/*//TODO should match above block if entities stays as grid coordinates, or below block if changed to world coordinates
	else if (collisionKey == 3) { // Entity collisions
	stopPlayerVertical();
	int gridX, gridY;
	worldToTileCoordinates(x1, y, &gridX, &gridY);
	player->position->y = gridToYBot(gridX, gridY) - player->height / 2.f; // fix penetration
	}*/
	/*//TODO NPC Collisions
	else if (isCollision(x, y) == 4) {
	//player->position->y =  - player->width / 2.f; // fix penetration
	}
	//Should never occur
	else if (isCollision(x, y) == 5) {}*/
}
//TODO Implement + Test
void Engine::collisionCheckDown() {
	//Check that the two grid blocks to the south are collision-free 
	float y = player->position->y - (player->height / 2.f);
	float x1 = player->position->x - (player->width / 2.f) + smallWorldUnit;
	int collisionKey1 = isCollision(x1, y);
	float x2 = player->position->x + (player->width / 2.f) - smallWorldUnit;
	int collisionKey2 = isCollision(x2, y);
	int collisionKey = collisionKeyOr(collisionKey1, collisionKey2);
	if (collisionKey <= 0){ //No collision OR collisions disabled
		/*string debug_string = "Move right: no collision\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		player->movingY = true; // Player is falling downwards
		player->isOnPlatform = false; // Player is not standing on a platform
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		//player->destination->x = x;
	}
	//TODO Change based on level design/mechanics
	// Currently places the player back on the map
	else if (collisionKey == 1) { // Out-of-bounds
		stopPlayerVertical();
		int gridX, gridY;
		worldToTileCoordinates(x1, y, &gridX, &gridY);
		player->position->y = gridToYTop(gridX, gridY) + player->height / 2.f;
		player->isOnPlatform = false; // Player is not standing on a platform
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
	}
	else if (collisionKey == 2) { // World (solids) collisions
		stopPlayerVertical();
		int gridX, gridY;
		worldToTileCoordinates(x1, y, &gridX, &gridY);
		player->position->y = gridToYTop(gridX, gridY) + player->height / 2.f; // fix penetration
		player->isOnPlatform = true; // Player is standing on a platform
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
	}
	/*//TODO should match above block if entities stays as grid coordinates, or below block if changed to world coordinates
	else if (collisionKey == 3) { // Entity collisions
		int gridX, gridY;
		worldToTileCoordinates(x1, y, &gridX, &gridY);
		player->position->y = gridToYTop(gridX, gridY) + player->height / 2.f; // fix penetration
	}*/
	/*//TODO NPC Collisions
	else if (isCollision(x, y) == 4) {
	//player->position->y =  + player->width / 2.f; // fix penetration
	}
	//Should never occur
	else if (isCollision(x, y) == 5) {}*/
}
void Engine::collisionCheckLeft() {
	//Check that the two grid blocks to the west are collision-free
	float y1 = player->position->y + (player->height / 2.f) - smallWorldUnit;
	float x = player->position->x - (player->width / 2.f);
	int collisionKey1 = isCollision(x, y1);
	float y2 = player->position->y - (player->height / 2.f) + smallWorldUnit;
	int collisionKey2 = isCollision(x, y2);
	/*string s = to_string(collisionKey2) + "\n"; //DEBUG
	OutputDebugString(s.data()); //DEBUG*/
	int collisionKey = collisionKeyOr(collisionKey1, collisionKey2);
	if (collisionKey <= 0){ //No collision OR collisions disabled
		/*string debug_string = "Move right: no collision\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		player->movingX = true;
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		//player->destination->x = x;
	}
	//TODO Change based on level design/mechanics
	// Currently places the player back on the map
	else if (collisionKey == 1) { // Out-of-bounds
		stopPlayerHorizontal();
		int gridX, gridY;
		worldToTileCoordinates(x, y1, &gridX, &gridY);
		player->position->x = gridToXRight(gridX, gridY) + player->width / 2.f;
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
	}
	else if (collisionKey == 2) { // World (solids) collisions
		stopPlayerHorizontal();
		int gridX, gridY;
		worldToTileCoordinates(x, y1, &gridX, &gridY);
		player->position->x = gridToXRight(gridX, gridY) + player->width / 2.f; // fix penetration
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
	}
	/*//TODO should match above block if entities stays as grid coordinates, or below block if changed to world coordinates
	else if (collisionKey == 3) { // Entity collisions
		int gridX, gridY;
		worldToTileCoordinates(x, y1, &gridX, &gridY);
		player->position->x = gridToXRight(gridX, gridY) + player->width / 2.f; // fix penetration
	}*/
	/*//TODO NPC Collisions
	else if (isCollision(x, y) == 4) {
	//player->position->x =  + player->width / 2.f; // fix penetration
	}
	//Should never occur
	else if (isCollision(x, y) == 5) {}*/
}
void Engine::collisionCheckRight() {
	//Check that the two grid blocks to the east are collision-free
	float y1 = player->position->y + player->height / 2.f - smallWorldUnit;
	float x = player->position->x + player->width / 2.f;
	int collisionKey1 = isCollision(x, y1);
	float y2 = player->position->y - player->height / 2.f + smallWorldUnit;
	int collisionKey2 = isCollision(x, y2);
	int collisionKey = collisionKeyOr(collisionKey1, collisionKey2);
	if (collisionKey <= 0){ //No collision OR collisions disabled
		/*string debug_string = "Move right: no collision\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		player->movingX = true;
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
		//player->destination->x = x;
	}
	//TODO Change based on level design/mechanics
	// Currently places the player back on the map
	else if (collisionKey == 1) { // Out-of-bounds
		stopPlayerHorizontal();
		int gridX, gridY;
		worldToTileCoordinates(x, y1, &gridX, &gridY);
		player->position->x = gridToXLeft(gridX, gridY) - player->width / 2.f;
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
	}
	else if (collisionKey == 2) { // World (solids) collisions
		stopPlayerHorizontal();
		int gridX, gridY;
		worldToTileCoordinates(x, y1, &gridX, &gridY);
		player->position->x = gridToXLeft(gridX, gridY) - player->width / 2.f; // fix penetration
		/*string debug_string = "player->position->x: " + to_string(player->position->x) + "\n"; //DEBUG
		OutputDebugString(debug_string.data()); //DEBUG*/
	}
	/*//TODO should match above block if entities stays as grid coordinates, or below block if changed to world coordinates
	else if (collisionKey == 3) { // Entity collisions
		int gridX, gridY;
		worldToTileCoordinates(x, y1, &gridX, &gridY);
		player->position->x = gridToXLeft(gridX, gridY) - player->width / 2.f; // fix penetration
	}*/
	/*//TODO NPC Collisions
	else if (isCollision(x, y) == 4) {
	//player->position->x =  - player->width / 2.f; // fix penetration
	}
	//Should never occur
	else if (isCollision(x, y) == 5) {}*/
}


void Engine::Update(float elapsed){
	//Apply gravity
	applyGravity(elapsed);
	//Update Player
	UpdatePlayer(elapsed);
	//Update NPCs
	UpdateNPCs(elapsed);
	

/*	string s = to_string(player->velocity->x) + "\n"; //DEBUG
	OutputDebugString(s.data()); //DEBUG*/
}
void Engine::UpdatePlayer(float elapsed){
	// Update player position
	player->position->x += player->velocity->x*elapsed;
	player->position->y += player->velocity->y*elapsed;

	// Player collision detection
	if (player->velocity->x > 0){
		collisionCheckRight();
	}
	if (player->velocity->x < 0){
		collisionCheckLeft();
	}
	if (player->velocity->y > 0) { //TEST
		collisionCheckUp();
	}
	else if (player->velocity->y < 0) {
		collisionCheckDown();
	}

	player->animationTime += elapsed;


	/*//Update player->isOnPlatform
	float x1 = player->position->x - (player->width / .2f);
	float y1 = player->position->y - smallWorldUnit;
	float x2 = player->position->x + (player->width / .2f);
	float y2 = player->position->y - smallWorldUnit;
	if (isCollision(x1, y1) == 2)*/
	

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

void Engine::applyGravity(float elapsed) {
	// Apply gravity to the player
	if (gravity != 0) { // Check for non-trivial gravity value
		if (player->velocity->y > term_vel) { // Check if current velocity is larger than term_vel
			player->velocity->y += gravity * elapsed;
			if (player->velocity->y < term_vel) {
				player->velocity->y = term_vel;
			}
		}
	}

	/*// TODO NPC gravity
	// Apply gravity to NPCs
	*/
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

