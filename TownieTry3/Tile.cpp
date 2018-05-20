#include "Tile.h"

vector<Tile> loadTileSet_default() {
	vector<Tile> tiles = vector<Tile>();
	tiles.push_back(Tile(0, 100));
	tiles.push_back(Tile(1, 4));
	tiles.push_back(Tile(2, 1));
	tiles.push_back(Tile(3, 1));
	tiles.push_back(Tile(4, 1));

	//Test tile - no custom graphics
	bool temp1[4] = { true, false, true, false };
	bool temp2[4] = { false, true, false, true };
	bool temp3[4] = { false, false, false, false };
	bool temp4[4] = { false, false, true, false };
	//tiles.push_back(Tile(&textures[0], 0.5f, temp4));
	return tiles;
}