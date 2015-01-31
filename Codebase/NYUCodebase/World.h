/*
This will hold the grid for each particular world.
Worlds Include
Home - 2 floors and a basement - closets and bedrooms
Dream - ?
School - two floors and a basement
Path to School - SubUrban land - able to circle the school.
*/

#pragma once
#include "SDL\SDL.h"
#include "SDL\SDL_opengl.h"
#include "SDL\SDL_image.h"

using namespace std;

class World{
public:
	World();
	World(const char *world_path,int spriteCountX, int spriteCountY,GLuint textureID, vector<int> solids);
	~World();
	int mapWidth;
	int mapHeight;
	int spriteCountX;
	int spriteCountY;
	GLuint textureID;

	vector<int> solids;// array of collision tiles in the given spritesheet
	unsigned char **levelData;
	bool readHeader(std::ifstream &stream);
	bool readLayerData(std::ifstream &stream);
};

World::~World(){}
World::World(){}
World::World(const char *world_path, int spriteCountX, int spriteCountY, GLuint textureID, vector<int> solids){
	World::spriteCountX = spriteCountX;
	World::spriteCountY = spriteCountY;
	World::textureID = textureID;
	World::solids = solids;

	ifstream infile(world_path);
	string line;
	while (getline(infile, line)){
		if (line == "[header]"){
			if (!readHeader(infile)){
				return;
			}
		} else if (line == "[layer]"){
			readLayerData(infile);
		}
	}
}

bool World::readHeader(std::ifstream &stream) {
	string line;
	mapWidth = -1;
	mapHeight = -1;
	while (getline(stream, line)) {
		if (line == "") { break; }
			istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
			if (key == "width") {
				mapWidth = atoi(value.c_str());
			}
			else if (key == "height"){
				mapHeight = atoi(value.c_str());
			}
	}
		if (mapWidth == -1 || mapHeight == -1) {
			return false;
		}
		else { // allocate our map data
			levelData = new unsigned char*[mapHeight];
			for (int i = 0; i < mapHeight; ++i) {
				levelData[i] = new unsigned char[mapWidth];
			}
			return true;
		}
}

bool World::readLayerData(std::ifstream &stream) {
	string line;
	while (getline(stream, line)) {
		if (line == "") { break; }
		istringstream sStream(line);
		string key, value;
		getline(sStream, key, '=');
		getline(sStream, value);
		if (key == "data") {
			for (int y = 0; y < mapHeight; y++) {
				getline(stream, line);
				istringstream lineStream(line);
				string tile;
				for (int x = 0; x < mapWidth; x++) {
					getline(lineStream, tile, ',');
					unsigned char val = (unsigned char)atoi(tile.c_str());
					if (val > 0) {
						// be careful, the tiles in this format are indexed from 1 not 0
						levelData[y][x] = val-1;
					}
					else {
						levelData[y][x] = 0;
					}
				}
			}
		}
	}
	return true;
}