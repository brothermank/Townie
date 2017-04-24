#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "Debugger.h"

Map::Map() {
	for (int y = 0; y < sy; y++) {
		vector<int> temp;
		vector<Structure*> temp1;
		tiles.push_back(temp);
		structures.push_back(temp1);
		for (int x = 0; x < sx; x++) {
			tiles[y].push_back(0);
			structures[y].push_back(0);
		}
	}
	name = "Random" + strh::toString(rand());
	nextAction();
}

Map::Map(vector<Tile> tilePalette) {
	for (int y = 0; y < sy; y++) {
		vector<int> temp;
		tiles.push_back(temp);
		vector<Structure*> temp1;
		structures.push_back(temp1);
		for (int x = 0; x < sx; x++) {
			tiles[y].push_back(0);
			structures[y].push_back(0);
		}
	}
	tileTypes = tilePalette;
	nextAction();
	name = "Random" + strh::toString(rand());
}
Map::Map(vector<Tile> tilePalette, string Name) {
	for (int y = 0; y < sy; y++) {
		vector<int> temp;
		tiles.push_back(temp);
		vector<Structure*> temp1;
		structures.push_back(temp1);
		for (int x = 0; x < sx; x++) {
			tiles[y].push_back(3);
			structures[y].push_back(0);
		}
	}
	tileTypes = tilePalette;
	nextAction();
	name = Name;
}

void Map::loadTiles(vector<Tile> tiles) {
	tileTypes = tiles;
}

Tile* Map::getTileAt(int x, int y) {
	int id;
	if (x < 0 || x > (int)tiles[y % tiles.size()].size() - 1 || y < 0 || y >(int)tiles.size() - 1) id = 0;
	else id = tiles[y][x];

	return &tileTypes[id];
}
int Map::getValueAt(int x, int y) {
	int id;
	if (x < 0 || x >(int)tiles[y % tiles.size()].size() - 1 || y < 0 || y >(int)tiles.size() - 1) id = 1;
	else id = tiles[y][x];
	return id;
}
void Map::setTileAt(int x, int y, int value) {
	if (x < 0 || x >(int)tiles[y % tiles.size()].size() - 1 || y < 0 || y >(int)tiles.size() - 1) return;
	if (tiles[y][x] != value) {
		actionStack.top().x.push_back(x);
		actionStack.top().y.push_back(y);
		actionStack.top().prevValue.push_back(tiles[y][x]);
		tiles[y][x] = value;
	}
}
Structure* Map::getStructureAt(int x, int y, bool br) {
	if (y >= 0 && y < (int)structures.size() && x >= 0 && x < (int)structures[y % structures.size()].size()) {
	return structures[x][y];
	}
	else {
	return NULL;
	}
	return 0;
}
int Map::setStructureAt(int x, int y, Structure* value) {
	if (x < 0 || x >(int)structures[y % structures.size()].size() - 1 || y < 0 || y >(int)structures.size() - 1) return -1;
	structures[x][y] = value;
	return 0;
}
void Map::removeStructureAt(int x, int y) {
	if (x < 0 || x >(int)structures[y % structures.size()].size() - 1 || y < 0 || y >(int)structures.size() - 1) {
		return;
	}
	structures[x][y] = 0;

}

double Map::getTravelWeightAt(int x, int y) {
	return getTileAt(x, y)->getTravelWeight();
}
double Map::getTravelWeightAt(Vector2 pos) {
	return getTileAt((int)pos.x, (int)pos.y)->getTravelWeight();
}
bool Map::getBorderDataAt(Vector2 from, Vector2 to, int borderIndex) {
	bool passable = getTileAt((int)from.x, (int)from.y)->borderEntrance[borderIndex % 4]
		&& getTileAt((int)to.x, (int)to.y)->borderEntrance[borderIndex + 2 % 4];
	Structure* s;
	if ((s = getStructureAt((int)from.x, (int)from.y)) != 0) {
		passable &= s->borderData[borderIndex % 4];
	}
	if ((s = getStructureAt((int)to.x, (int)to.y)) != 0) {
		passable &= s->borderData[(borderIndex + 2) % 4];
	}

	return passable;
}
bool Map::getBorderDataAt(int xfrom, int yfrom, int xto, int yto, int borderIndex) {
	bool passable = getTileAt(xfrom, yfrom)->borderEntrance[borderIndex % 4]
		&& getTileAt(xto, yto)->borderEntrance[borderIndex + 2 % 4];
	Structure* s;
	if ((s = getStructureAt(xfrom, yfrom)) != 0) {
		passable &= s->borderData[borderIndex % 4];
	}
	if ((s = getStructureAt(xto, yto)) != 0) {
		passable &= s->borderData[(borderIndex + 2) % 4];
	}

	return passable;
}
bool Map::getBorderDataAt(Vector2 from, int xto, int yto, int borderIndex) {
	bool passable = getTileAt((int)from.x, (int)from.y)->borderEntrance[borderIndex % 4]
		&& getTileAt(xto, yto)->borderEntrance[(borderIndex + 2) % 4];
	Structure* s;
	if ((s = getStructureAt((int)from.x, (int)from.y)) != 0) {
		passable &= s->borderData[borderIndex % 4];
		Debugger::print("Exiting structure at pos " + strh::toString(from.x) + "," + strh::toString(from.y) + "   values: " + strh::toString(s->borderData[0]) + "," + strh::toString(s->borderData[1]) + "," + strh::toString(s->borderData[2]) + "," + strh::toString(s->borderData[3]) + "\n");
	}
	if ((s = getStructureAt(xto, yto)) != 0) {
		Debugger::print("Entering structure at pos " + strh::toString(xto) + "," + strh::toString(yto) + "   values: " + strh::toString(s->borderData[0]) + "," + strh::toString(s->borderData[1]) + "," + strh::toString(s->borderData[2]) + "," + strh::toString(s->borderData[3]) + "\n");
		passable &= s->borderData[(borderIndex + 2) % 4];
	}

	return passable;
}
bool Map::isSafe(int x, int y) {
	Structure* s;
	if ((s = getStructureAt(x, y, true)) != 0) {
		return s->isSafe;
	}
	else return false;
}


void Map::undoLastChange() {
	if (actionStack.size() > 1) {
		actionStack.pop();
		Action a = actionStack.top();
		for (size_t i = 0; i < a.x.size(); i++) {
			tiles[a.x[i]][a.y[i]] = a.prevValue[i];
		}
		actionStack.pop();
		nextAction();
	}

}
void Map::nextAction() {
	if (actionStack.size() <= 0) {
		actionStack.push(Action());
	}
	else if (actionStack.top().x.size() > 0) {
		actionStack.push(Action());
	}
}

void Map::save() {
	vector<string> lines;
	for (size_t y = 0; y < tiles.size(); y++) {
		string line = "";
		for (size_t x = 0; x < tiles[y].size(); x++) {
			line += " " + strh::toString(getValueAt(x, y));
		}
		line += " ;";
		lines.push_back(line);
	}
	ofstream file;
	string path = "Saves/Maps/" + strh::toString(name) + ".cmap";
	file.open(path);
	for (unsigned int i = 0; i < lines.size(); i++) {
		lines[i] = lines[i] + "\n";
		file << lines[i].c_str();
	}
}

void Map::load(string mapName) {
	vector<vector<int>> temp;
	tiles = temp;
	std::string::size_type sz;

	ifstream input("Saves/Maps/" + mapName + fileending);
	string line;
	int i = 0;
	while (getline(input, line)) {
		vector<int> temp;
		vector<Structure*> temp1;
		tiles.push_back(temp);
		structures.push_back(temp1);

		size_t a = 0, b = 0;
		size_t c;
		c = string::npos;
		while (true) {
			a = line.find(" ", b);
			if (a == string::npos) break;
			a++;
			b = line.find(" ", a);
			if (b == string::npos) break;
			string nextstring = line.substr(a, b - a);
			int nextint = 0;
			if (nextstring.size() == 0) nextint = 3;
			else nextint = stoi(nextstring, &sz);
			tiles[i].push_back(nextint);
			structures[i].push_back(0);
		}
		i++;
	}
}


Map LoadMap(string mapName) {
	Map map = Map::Map();
	map.load(mapName);
	return map;
}

double Tile::getTravelWeight() {
	return travelWeight;
}