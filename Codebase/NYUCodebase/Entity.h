#pragma once
#include "SheetSprite.h"
#include <vector>

class Entity {
public:
	Entity();
	Entity(SheetSprite sprite, Vector position, Vector scale);

	SheetSprite sprite;							

	//	Basic Functions
	void Update(float elapsed);
	void Render();
	bool CollidesWith(Entity *entity);
	void FixedUpdate();

	//	Variables required for SheetSprite::Draw
	Vector position;
	Vector scale;
	float rotation = 0.0f;
	bool flipped = false;	

	//	Animations: must be updated in the Update function
	float timeToComplete = 0.0f;				//	Total time we have to complete the animations	(MUST BE GIVEN)
	float timeOnFrame = 0.0f;					//	Time spent on the current frame
	int frameIndex = 0;							//	Starting frame index is 0
	std::vector<SheetSprite> animationFrames;	//	Vector of animation frames						(MUST BE GIVEN)

	//	Collsion Detection 

	enum CollisionFlags { COLLISION_TOP, COLLISION_BOTTOM, COLLISION_LEFT, COLLISION_RIGHT };						
};