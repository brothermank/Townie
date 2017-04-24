#pragma once
#include "Vector.h"
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <memory>
#include <iterator>

class MapWindow;

using namespace std;

struct Node {

	Vector2 pos;
	double size;
	Node * previousNode;

	Node() :pos(0, 0), size(0), previousNode(NULL) {}
	Node(double xinit, double yinit, double sizeinit) :pos(xinit, yinit), size(sizeinit), previousNode(NULL) {}

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
	Path(vector<Node> n) :nodes(n) {};
	size_t size() { return nodes.size(); }
	int pathLength();

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

class Navigator {
public:
	Navigator();
	Navigator(MapWindow * mapWindow);
	Path findPath(Vector2 startpos, Vector2 destinationPos);
	MapWindow * world;

private:
	vector<Node> findUnexploredSurroundingPaths(Vector2 pos);
	Node findShortestNode(vector<Node> nodes, bool frontierNodes = false);
	Node findShortestSurroundingPaths(Node n);
	vector<vector<bool>> flags;
	int cIndex;
	bool deadEnd;
};

