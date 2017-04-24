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
	int getValueAt(int x, int y);
	Tile* getTileAt(int x, int y);
	void setTileAt(int x, int y, int value);
	Structure* getStructureAt(int x, int y, bool br = false);
	int setStructureAt(int x, int y, Structure* value);
	void removeStructureAt(int x, int y);
	double getTravelWeightAt(int x, int y);
	double getTravelWeightAt(Vector2 pos);
	bool getBorderDataAt(Vector2 from, Vector2 to, int borderIndex);
	bool getBorderDataAt(int xfrom, int yfrom, int xto, int yto, int borderIndex);
	bool getBorderDataAt(Vector2 from, int xto, int yto, int borderIndex);
	bool isSafe(int x, int y);

	void undoLastChange();
	void nextAction();
	void save();
	void load(string mapName);

	vector<vector<int>> tiles;
	vector<vector<Structure*>> structures;
	std::vector<Tile> tileTypes;
	string name = "";

private:
	stack<Action> actionStack;

};

Map LoadMap(string mapName);

