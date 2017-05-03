#include "Map.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
#include "Debugger.h"

Map::Map() {
	sempty = make_shared<Structure>(Structure());
	for (int y = 0; y < sy; y++) {
		vector<int> temp;
		vector<shared_ptr<Structure>> temp1;
		tiles.push_back(temp);
		structures.push_back(temp1);
		for (int x = 0; x < sx; x++) {
			tiles[y].push_back(0);
			structures[y].push_back(sempty);
		}
	}
	sizex = sx; sizey = sy;
	name = "Random" + strh::toString(rand());
	nextAction();
}
Map::Map(vector<Tile> tilePalette, string mapName, string loadName) {
	sempty = make_shared<Structure>(Structure());
	load(loadName);
	sizex = sx; sizey = sy;
	tileTypes = tilePalette;
	nextAction();
	name = mapName;
}

void Map::loadTiles(vector<Tile> tiles) {
	tileTypes = tiles;
}

void Map::editorSetTileAt(size_t x, size_t y, size_t value) {
	if (y < sizey - 1 && x < sizex - 1 && tiles[y][x] != value) {
		actionStack.top().x.push_back(x);
		actionStack.top().y.push_back(y);
		actionStack.top().prevValue.push_back(tiles[y][x]);
		tiles[y][x] = value;
	}
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
	for (size_t y = 0; y < sizey; y++) {
		string line = "";
		for (size_t x = 0; x < sizex; x++) {
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
	sizey = 0;

	ifstream input("Saves/Maps/" + mapName + fileending);
	string line;
	int i = 0;
	while (getline(input, line)) {
		vector<int> temp;
		vector<shared_ptr<Structure>> temp1;
		tiles.push_back(temp);
		structures.push_back(temp1);
		sizey++;

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
			structures[i].push_back(sempty);
		}
		i++;
	}
	sizex = tiles[0].size();
	int qa = 0;
}


Map LoadMap(string mapName) {
	Map map = Map::Map();
	map.load(mapName);
	return map;
}

double Tile::getTravelWeight() {
	return travelWeight;
}