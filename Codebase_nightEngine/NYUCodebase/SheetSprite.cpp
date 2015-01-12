#include "SheetSprite.h"

SheetSprite::SheetSprite() {}
SheetSprite::SheetSprite(unsigned int textureID, float u, float v, float width, float height) : textureID(textureID), u(u), v(v), width(width), height(height) {}

/*	Draw the object to the screen with the provided parameters
*/
void SheetSprite::Draw(Vector position, Vector scale, float rotation, bool flipped) {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(position.x, position.y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	if (flipped){
		scale.x *= -1.0f;
	}
	
	GLfloat quad[] = { -width * scale.x, height * scale.y, -width * scale.x, -height * scale.y, width * scale.x, -height * scale.y, width * scale.x, height * scale.y };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { u, v, u, v + height, u + width, v + height, u + width, v };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}
