#pragma once
#define MapMaxSizeX 40
#define MapMaxSizeY 40

#include "Vector.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>
#include <iterator>

class MapWindow;
class Map;

using namespace std;

struct Node {

	Vector2ST pos;
	double size;
	Node * previousNode;

	Node() :pos(0, 0), size(0), previousNode(NULL) {}
	Node(size_t xinit, size_t yinit, double sizeinit) :pos(xinit, yinit), size(sizeinit), previousNode(NULL) {}

	Node clone() {
		Node temp;
		temp.pos = pos;
		temp.size = size;
		temp.previousNode = previousNode;
		return temp;
	}
};

class Path {
public:
	vector<Node> nodes;
	Path();
	Path(vector<Node> n) {
		nodes.reserve(n.size());
		for (size_t i = 0; i < n.size(); i++) {
			nodes.push_back(n[i]);
		}
	};
	Path(vector<Node*> n) {
		nodes.reserve(n.size());
		for (size_t i = 0; i < n.size(); i++) {
			nodes.push_back(*n[i]);
		}
	};
	size_t size() { return nodes.size(); }
	double pathLength();

	void push_back(Node node) { nodes.push_back(node); }

	/**Removes the first node from node array*/
	void pop() { nodes.erase(nodes.begin()); }

	Node& operator[] (int x) {
		return nodes[x];
	}
	Path& operator= (vector<Node> n) {
		nodes = n;
		return *this;
	}

};

struct NextFrontier {
	Node nodes[4];
	bool inFrontier[4];

	NextFrontier() {
		for (size_t i = 0; i < 4; i++) {
			nodes[i] = Node(0, 0, 0);
			inFrontier[i] = false;
		}
	}
};


class Navigator {
public:
	Navigator();
	Navigator(MapWindow * mapWindow);
	Path findPath(Vector2D startpos, Vector2ST destinationPos);
	MapWindow * world;

	static Node referenceNodes[40][40];
	static bool flags[40][40];
private:
	void findUnexploredSurroundingPaths(Vector2ST pos, NextFrontier* nf);
	Node* findShortestNode(vector<Node*> nodes);
	int cIndex;
	bool deadEnd;

	shared_ptr<Map> map;
	size_t nodesSize;

	size_t stTemp0;
	double dtemp0;
	Node * current;

	int i1, i2, i3, i4, i5, i6, i7, i8, i9;
	//i1: Map initializer
	//i2: Initial node flags
	//i3: Main loop iterations (frontiers searched)
	//i4: Frontiers found/nodes explored (if inside of main while loop)
	//i5: Nodes searched through for shortest nodes
	//i6: shortest node index reassigns
	//i7: shortest node searches
	//i8: Find new nodes calls
	//i9: Nodes found

};


