#pragma once
#include <SDL.h>
#include <vector>
#include <stack>
#include "StringHelper.h"
#include "ResourceLoader.h"
#include "Debugger.h"
#include "Structure.h"
#include <memory>
#include <chrono>

#define sx 10
#define sy 20
#define fileending ".cmap"

using namespace std;

struct Action {
	vector<int> x;
	vector<int> y;
	vector<int> prevValue;
};

struct Tile {
public:
	double getTravelWeight();

	unsigned int sizeX;
	unsigned int sizeY;
	bool borderEntrance[4];
	LTexture* texture;

	Tile(LTexture* text, double ntravelWeight = 1, bool *borderEntrance = 0, Structure s = Structure()) {
		texture = text;
		sizeX = text->mWidth;
		sizeY = text->mHeight;
		travelWeight = ntravelWeight;
		for (int i = 0; i < 4; i++) {
			if (borderEntrance != 0) {
				this->borderEntrance[i] = borderEntrance[i];
			}
			else {
				this->borderEntrance[i] = true;
			}
		}
	}

private:
	double travelWeight;
};

class Map {
public:
	Map();
	Map(vector<Tile> tilePalette, string mapName, string loadName);
	void loadTiles(vector<Tile> textures);
	void editorSetTileAt(size_t x, size_t y, size_t value);
	
	void undoLastChange();
	void nextAction();
	void save();
	void load(string mapName);

	Structure sempty;

	vector<vector<int>> tiles;
	std::vector<Tile> tileTypes;
	string name = "";

	size_t sizex, sizey;

	//Inline functions

	inline Tile* Map::getTileAtSafe(size_t x, size_t y) {
		if (y < sizey - 1 && x < sizex - 1) return &tileTypes[tiles[y][x]];
		else return &tileTypes[0];

	}
	inline Tile* Map::getTileAt(size_t x, size_t y) {
		return &tileTypes[tiles[y][x]];
	}
	inline int Map::getValueAtSafe(size_t x, size_t y) {
		if (y < sizey - 1 && x < sizex - 1) return tiles[y][x];
		else return 1;
	}
	inline int Map::getValueAt(size_t x, size_t y) {
		return tiles[y][x];
	}

	inline Structure* Map::getStructureAtSafe(size_t x, size_t y) {
		if (y < sizey &&  x < sizex) return structures[x][y];
		else return NULL;
	}
	inline Structure* Map::getStructureAt(size_t x, size_t y) {
		return structures[x][y];
	}
	inline bool Map::setStructureAtSafe(size_t x, size_t y, Structure* newStruct) {
		if (y < sizey && x < sizex) structures[x][y] = newStruct;
		else return false;
		return true;
	}
	inline void Map::setStructureAt(size_t x, size_t y, Structure* newStruct) {
		structures[x][y] = newStruct;
	}
	inline void Map::removeStructureAtSafe(size_t x, size_t y) {
		if (y < sizey && x < sizex) {
			structures[x][y] = &sempty;
		}
	}
	inline void Map::removeStructureAt(size_t x, size_t y) {
		structures[x][y] = &sempty;
	}

	inline double Map::getTravelWeightAtSafe(size_t x, size_t y) {
		return getTileAtSafe(x, y)->getTravelWeight();
	}
	inline double Map::getTravelWeightAt(size_t x, size_t y) {
		return getTileAt(x, y)->getTravelWeight();
	}
	inline double Map::getTravelWeightAtSafe(Vector2ST pos) {
		return getTileAtSafe(pos.x, pos.y)->getTravelWeight();
	}
	inline double Map::getTravelWeightAt(Vector2ST pos) {
		return getTileAt(pos.x, pos.y)->getTravelWeight();
	}
	inline bool Map::getBorderDataAtSafe(Vector2ST from, Vector2ST to, size_t borderIndex) {
		getBorderDataAtSafe(from.x, from.y, to.x, to.y, borderIndex);

	}
	inline bool Map::getBorderDataAtSafe(size_t xfrom, size_t yfrom, size_t xto, size_t yto, size_t borderIndex) {
		return getTileAtSafe(xfrom, yfrom)->borderEntrance[borderIndex]
			&& getTileAtSafe(xto, yto)->borderEntrance[borderIndex + 2]
			&& getStructureAtSafe(xfrom, yfrom)->borderData[borderIndex]
			&& getStructureAtSafe(xto, yto)->borderData[borderIndex + 2];

	}
	inline bool Map::getBorderDataAtSafe(Vector2ST from, size_t xto, size_t yto, size_t borderIndex) {
		getBorderDataAtSafe(from.x, from.y, xto, yto, borderIndex);
	}
	inline bool Map::getBorderDataAt(size_t xfrom, size_t yfrom, size_t xto, size_t yto, size_t borderIndex) {
		return getTileAt(xfrom, yfrom)->borderEntrance[borderIndex]
			&& getTileAt(xto, yto)->borderEntrance[borderIndex + 2]
			&& getStructureAt(xfrom, yfrom)->borderData[borderIndex]
			&& getStructureAt(xto, yto)->borderData[borderIndex + 2];
	}
	
	inline bool Map::isSafeSafe(size_t x, size_t y) {
		Structure * s = getStructureAtSafe(x, y);
		bool safe = s->isSafe;
		return safe;
	}
	inline bool Map::isSafe(size_t x, size_t y) {
		return getStructureAt(x, y)->isSafe;
	}



private:
	stack<Action> actionStack;
	vector<vector<Structure*>> structures;


	bool btemp0, btemp1;
	Structure *stemp0, *stemp1;

	
};

Map LoadMap(string mapName);

