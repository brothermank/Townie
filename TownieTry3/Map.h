#pragma once
#include <SDL.h>
#include <vector>
#include <stack>
#include "StringHelper.h"
#include "ResourceLoader.h"
#include "Debugger.h"
#include "Structure.h"
#include <memory>

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
	Map(vector<Tile> textures);
	Map(vector<Tile> textures, string name);
	void loadTiles(vector<Tile> textures);
	void editorSetTileAt(size_t x, size_t y, size_t value);
	
	void undoLastChange();
	void nextAction();
	void save();
	void load(string mapName);

	vector<vector<int>> tiles;
	vector<vector<Structure*>> structures;
	std::vector<Tile> tileTypes;
	string name = "";


	//Inline functions

	inline Tile* Map::getTileAt(size_t x, size_t y) {
		if (y < tiles.size() - 1 && x < tiles[y].size() - 1) return &tileTypes[tiles[y][x]];
		else return &tileTypes[0];

	}
	inline int Map::getValueAt(size_t x, size_t y) {
		if (y < tiles.size() - 1 && x < tiles[y].size() - 1) return tiles[y][x];
		else return 1;
	}

	inline Structure* Map::getStructureAt(size_t x, size_t y) {
		if (y < structures.size() &&  x < structures[y % structures.size()].size()) {
			return structures[x][y];
		} else {
			return NULL;
		}
	}
	inline bool Map::setStructureAt(size_t x, size_t y, Structure* value) {
		if (y < structures.size() && x < structures[y % structures.size()].size()) structures[x][y] = value;
		else return false;
		return true;
	}
	inline void Map::removeStructureAt(size_t x, size_t y) {
		if (y < structures.size() && x < structures[y % structures.size()].size()) {
			structures[x][y] = NULL;
		}
	}

	inline double Map::getTravelWeightAt(size_t x, size_t y) {
		return getTileAt(x, y)->getTravelWeight();
	}
	inline double Map::getTravelWeightAt(Vector2ST pos) {
		return getTileAt(pos.x, pos.y)->getTravelWeight();
	}
	inline bool Map::getBorderDataAt(Vector2ST from, Vector2ST to, size_t borderIndex) {
		btemp0 = getTileAt(from.x, from.y)->borderEntrance[borderIndex]
			&& getTileAt(to.x, to.y)->borderEntrance[(borderIndex + 2) % 4];
		if ((stemp0 = getStructureAt(from.x, from.y)) != NULL) {
			btemp0 &= stemp0->borderData[borderIndex];
		}
		if ((stemp0 = getStructureAt(to.x, to.y)) != NULL) {
			btemp0 &= stemp0->borderData[(borderIndex + 2) % 4];
		}

		return btemp0;
	}
	inline bool Map::getBorderDataAt(size_t xfrom, size_t yfrom, size_t xto, size_t yto, size_t borderIndex) {
		btemp0 = getTileAt(xfrom, yfrom)->borderEntrance[borderIndex]
			&& getTileAt(xto, yto)->borderEntrance[(borderIndex + 2) % 4];
		if ((stemp0 = getStructureAt(xfrom, yfrom)) != NULL) {
			btemp0 &= stemp0->borderData[borderIndex];
		}
		if ((stemp0 = getStructureAt(xto, yto)) != NULL) {
			btemp0 &= stemp0->borderData[(borderIndex + 2) % 4];
		}

		return btemp0;
	}
	inline bool Map::getBorderDataAt(Vector2ST from, size_t xto, size_t yto, size_t borderIndex) {
		btemp0 = getTileAt(from.x, from.y)->borderEntrance[borderIndex]
			&& getTileAt(xto, yto)->borderEntrance[(borderIndex + 2) % 4];
		if ((stemp0 = getStructureAt(from.x, from.y)) != NULL) {
			btemp0 &= stemp0->borderData[borderIndex];
		}
		if ((stemp0 = getStructureAt(xto, yto)) != NULL) {
			btemp0 &= stemp0->borderData[(borderIndex + 2) % 4];
		}

		return btemp0;
	}
	inline bool Map::isSafe(size_t x, size_t y) {
		if ((stemp1 = getStructureAt(x, y)) != 0) {
			return stemp1->isSafe;
		} else return false;
	}


private:
	stack<Action> actionStack;


	bool btemp0, btemp1;
	Structure *stemp0, *stemp1;
};

Map LoadMap(string mapName);

