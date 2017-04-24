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
	void editorSetTileAt(int x, int y, int value);
	
	void undoLastChange();
	void nextAction();
	void save();
	void load(string mapName);

	vector<vector<int>> tiles;
	vector<vector<Structure*>> structures;
	std::vector<Tile> tileTypes;
	string name = "";


	//Inline functions

	inline Tile* Map::getTileAt(int x, int y) {
		if (x < 0 || x >(int)tiles[y % tiles.size()].size() - 1 || y < 0 || y >(int)tiles.size() - 1) return &tileTypes[0];
		else return &tileTypes[tiles[y][x]];

	}
	inline int Map::getValueAt(int x, int y) {
		if (x < 0 || x >(int)tiles[y % tiles.size()].size() - 1 || y < 0 || y >(int)tiles.size() - 1) return 1;
		else return tiles[y][x];
	}

	inline Structure* Map::getStructureAt(int x, int y) {
		if (y >= 0 && y < (int)structures.size() && x >= 0 && x < (int)structures[y % structures.size()].size()) {
			return structures[x][y];
		} else {
			return NULL;
		}
	}
	inline int Map::setStructureAt(int x, int y, Structure* value) {
		if (x < 0 || x >(int)structures[y % structures.size()].size() - 1 || y < 0 || y >(int)structures.size() - 1) return -1;
		structures[x][y] = value;
		return 0;
	}
	inline void Map::removeStructureAt(int x, int y) {
		if (x < 0 || x >(int)structures[y % structures.size()].size() - 1 || y < 0 || y >(int)structures.size() - 1) {
			return;
		}
		structures[x][y] = NULL;

	}

	inline double Map::getTravelWeightAt(int x, int y) {
		return getTileAt(x, y)->getTravelWeight();
	}
	inline double Map::getTravelWeightAt(Vector2 pos) {
		return getTileAt((int)pos.x, (int)pos.y)->getTravelWeight();
	}
	inline bool Map::getBorderDataAt(Vector2 from, Vector2 to, int borderIndex) {
		btemp0 = getTileAt((int)from.x, (int)from.y)->borderEntrance[borderIndex % 4]
			&& getTileAt((int)to.x, (int)to.y)->borderEntrance[borderIndex + 2 % 4];
		if ((stemp0 = getStructureAt((int)from.x, (int)from.y)) != NULL) {
			btemp0 &= stemp0->borderData[borderIndex % 4];
		}
		if ((stemp0 = getStructureAt((int)to.x, (int)to.y)) != NULL) {
			btemp0 &= stemp0->borderData[(borderIndex + 2) % 4];
		}

		return btemp0;
	}
	inline bool Map::getBorderDataAt(int xfrom, int yfrom, int xto, int yto, int borderIndex) {
		btemp0 = getTileAt(xfrom, yfrom)->borderEntrance[borderIndex % 4]
			&& getTileAt(xto, yto)->borderEntrance[borderIndex + 2 % 4];
		if ((stemp0 = getStructureAt(xfrom, yfrom)) != NULL) {
			btemp0 &= stemp0->borderData[borderIndex % 4];
		}
		if ((stemp0 = getStructureAt(xto, yto)) != NULL) {
			btemp0 &= stemp0->borderData[(borderIndex + 2) % 4];
		}

		return btemp0;
	}
	inline bool Map::getBorderDataAt(Vector2 from, int xto, int yto, int borderIndex) {
		btemp0 = getTileAt((int)from.x, (int)from.y)->borderEntrance[borderIndex % 4]
			&& getTileAt(xto, yto)->borderEntrance[(borderIndex + 2) % 4];
		if ((stemp0 = getStructureAt((int)from.x, (int)from.y)) != NULL) {
			btemp0 &= stemp0->borderData[borderIndex % 4];
		}
		if ((stemp0 = getStructureAt(xto, yto)) != NULL) {
			btemp0 &= stemp0->borderData[(borderIndex + 2) % 4];
		}

		return btemp0;
	}
	inline bool Map::isSafe(int x, int y) {
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

