#pragma once
#include "ResourceHandler.h"
#include "Structure.h"

struct Tile {
public:
	double getTravelWeight();

	unsigned int sizeX;
	unsigned int sizeY;
	bool borderEntrance[8];
	int textureID;

	Tile(int textureID, double ntravelWeight = 1, bool *borderEntrance = 0, Structure s = Structure()) : textureID(textureID) {
		travelWeight = ntravelWeight;
		for (int i = 0; i < 4; i++) {
			if (borderEntrance != 0) {
				this->borderEntrance[i] = borderEntrance[i];
				this->borderEntrance[i + 4] = borderEntrance[i];
			}
			else {
				this->borderEntrance[i] = true;
				this->borderEntrance[i + 4] = true;
			}
		}
	}

private:
	double travelWeight;
};

vector<Tile> loadTileSet_default();