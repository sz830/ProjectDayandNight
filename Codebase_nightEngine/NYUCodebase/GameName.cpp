#include "GameName.h"

/*	This function sets up the basics for graphical displays on the screen.
*/
GameName::GameName() {
	ScreenResolution.x = 800;
	ScreenResolution.y = 600;
	aspectRatio = (float)ScreenResolution.x / ScreenResolution.y;

	SDL_Init(SDL_INIT_VIDEO);
	displayWindow = SDL_CreateWindow("GameName", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, ScreenResolution.x, ScreenResolution.y, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);

	//--- Basic setup for the rendering pipeline
	glViewport(0, 0, ScreenResolution.x, ScreenResolution.y);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-aspectRatio, aspectRatio, -1.0, 1.0, -1.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
	Init();
}

/*	This function is run once at the start of the game.
	It loads in textures, music, sounds, and generates the Main Menu.
*/
void GameName::Init() {
	//======================================================================================================================
	//--- Load in textures
	//		textureName = LoadTexture("fileName.png");
	//======================================================================================================================

	//======================================================================================================================
	//--- Load in Music / Sounds
	//======================================================================================================================

	//======================================================================================================================
	//--- Set up the main menu
	//======================================================================================================================
}

/*	~GameName()
	Called when the game should be closed.
*/
GameName::~GameName() {
	//======================================================================================================================
	//--- Free the Music / Sound Assets that were loaded in
	//		In the following code, the music tracks that were loaded in for the game are freed. 
	//======================================================================================================================
	//Mix_FreeMusic(musicName);


	//Mix_FreeChunk(soundName);

	//======================================================================================================================
	//--- Close the game
	//======================================================================================================================
	SDL_Quit();
}

/*	This function renders the game objects onto the screen based on the state of the game
	There are four states to consider: the main menu, the white player's turn, the black player's turn and game over
*/
void GameName::Render() {
	glClearColor(0.3f, 0.6f, 0.9f, 1.0f); // Default Clear Screen
	glClear(GL_COLOR_BUFFER_BIT);


	SDL_GL_SwapWindow(displayWindow);
}

void GameName::Update(float elapsed) {
	
}

void GameName::FixedUpdate(){

}

bool GameName::UpdateAndRender() {

	//	Updating the timing of the game
	float ticks = (float)SDL_GetTicks() / 1000.0f;
	float elapsed = ticks - lastFrameTicks;
	lastFrameTicks = ticks;

	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			done = true;
		}
		else if (event.type == SDL_MOUSEMOTION) {
			mousePosition.x = (((float)event.motion.x / ScreenResolution.x) * 2 * aspectRatio) - aspectRatio;
			mousePosition.y = (((float)(ScreenResolution.y - event.motion.y) / ScreenResolution.y) * 2.0f) - 1.0f;
		}
	}

	// Keyboard Input 
	const Uint8 *keys = SDL_GetKeyboardState(NULL);
	if (keys[SDL_SCANCODE_LEFT]) {
		
	}
	if (keys[SDL_SCANCODE_RIGHT]) {
	
	}
	if (keys[SDL_SCANCODE_UP]) {
		
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		
	}
	
	float fixedElapsed = elapsed + timeLeftOver;
	if (fixedElapsed > FIXED_TIMESTEP * MAX_TIMESTEPS) {
		fixedElapsed = FIXED_TIMESTEP * MAX_TIMESTEPS;
	}
	while (fixedElapsed >= FIXED_TIMESTEP) {
		fixedElapsed -= FIXED_TIMESTEP;
		FixedUpdate();
	}
	timeLeftOver = fixedElapsed;

	Update(elapsed);	
	Render();

	return done;
}

/*	Lerp Function
*/
float GameName::lerp(float v0, float v1, float t) {
	return (1.0f - t)*v0 + t*v1;
}

/*	Prints strings to the screen
*/
void GameName::Text(GLuint Texture, string text, float size, float spacing, float r, float g, float b, float a) {
	// Have to go character by character for the offsets...
	// The 16 is the width of the sprite sheet in terms of the number of sprites
	// We must draw multiple quads -- more complex than a single square -- dynamic mesh

	glBindTexture(GL_TEXTURE_2D, Texture);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	vector<float> vertexData;
	vector<float> uvData;
	vector<float> colorData;

	// We must build vertex by vertex for each quad ...
	// We must account for the offset between letters by using size*i
	// We're transforming manually. We're drawing all the quads in a single draw column.
	// If we used the modelview matrix, the entire thing is affected. We don't want that...
	float texture_size = 1.0f / 16.0f;

	for (size_t i = 0; i < text.size(); i++) {
		float texture_x = (float)(((int)text[i]) % 16) / 16.0f;
		float texture_y = (float)(((int)text[i]) / 16) / 16.0f;

		vertexData.insert(vertexData.end(), { ((size + spacing) * i) + (-0.5f* size), (0.5f * size),
			((size + spacing) * i) + (-0.5f* size), (-0.5f * size),
			((size + spacing) * i) + (0.5f* size), (-0.5f * size),
			((size + spacing) * i) + (0.5f* size), (0.5f * size) });
		colorData.insert(colorData.end(), { r, g, b, a, r, g, b, a, r, g, b, a, r, g, b, a });
		uvData.insert(uvData.end(), { texture_x, texture_y, texture_x, texture_y + texture_size, texture_x + texture_size, texture_y + texture_size, texture_x + texture_size, texture_y });
	}

	glVertexPointer(2, GL_FLOAT, 0, vertexData.data());
	glEnableClientState(GL_VERTEX_ARRAY);

	glTexCoordPointer(2, GL_FLOAT, 0, uvData.data());
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glColorPointer(4, GL_FLOAT, 0, colorData.data());
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, text.size() * 4);
	glDisableClientState(GL_COLOR_ARRAY);
}