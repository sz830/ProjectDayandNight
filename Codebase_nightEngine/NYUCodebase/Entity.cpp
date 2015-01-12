#include "Entity.h"

Entity::Entity() {}
Entity::Entity(SheetSprite sprite, Vector position, Vector scale) : sprite(sprite), position(position), scale(scale) {}

/*	This function updates the Entity
*/
void Entity::Update(float elapsed) {

}

//	This function will draw the Entity on the screen using SheetSprite::Draw
void Entity::Render() {
	sprite.Draw(position, scale, rotation, flipped);
}

bool Entity::CollidesWith(Entity *entity) {

	// Create variables for the top, bottom, left, and right of the argument objects
	float entity_top = position.y + (sprite.height / 2.0f);
	float entity_bot = position.y - (sprite.height / 2.0f);
	float entity_right = position.x + (sprite.width / 2.0f);
	float entity_left = position.x - (sprite.width / 2.0f);

	float bullet_top = entity->position.y + (entity->sprite.height / 2.0f);
	float bullet_bot = entity->position.y - (entity->sprite.height / 2.0f);
	float bullet_right = entity->position.x + (entity->sprite.width / 2.0f);
	float bullet_left = entity->position.x - (entity->sprite.width / 2.0f);

	// Based on the rules for collision detection we have:
	if (entity_bot > bullet_top) {
		return false;
	}
	else if (entity_top < bullet_bot) {
		return false;
	}
	else if (entity_left > bullet_right){
		return false;
	}
	else if (entity_right < bullet_left) {
		return false;
	}

	// A true is returned if there has been a collision
	return true;
}