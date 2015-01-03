/*	
	Shun Man Tse
	Dec. 2014
	Game Programming Boilerplate Code
*/

#include "GameName.h"

SDL_Window* displayWindow;

int main(int argc, char *argv[]) {
	GameName app;
	while (!app.UpdateAndRender()) {}
	return 0;
}

/* Asset Credits

*/