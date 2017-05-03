#include "Debugger.h"
#include "StringHelper.h"
#include "MapWindow.h"
#include "Navigator.h"
#include <time.h>
#include <chrono>

using namespace std;

Path::Path() {}
double Path::pathLength() {
	int i = nodes[nodes.size() - 1].size;
	return nodes[nodes.size() - 1].size;
}



Navigator::Navigator() {
}
Navigator::Navigator(MapWindow * mapWindow) {
	world = mapWindow;
}


void resetFlags() {
	for (int x = 0; x < MapMaxSizeX; x++) {
		for (int y = 0; y < MapMaxSizeY; y++) {
			Navigator::flags[y][x] = false;
			Navigator::referenceNodes[y][x] = Node(x,y,0);
		}
	}
}
Path Navigator::findPath(Vector2D startpos, Vector2ST destinationPos) {
	resetFlags();
	//initialization
	//Define variables
	i1 = 0; i2 = 0; i3 = 0; i4 = 0; i5 = 0; i6 = 0; i7 = 0; i8 = 0; i9 = 0;

	vector<Node*> nodes;//all the frontier nodes with more paths to explore
	size_t x = (size_t)(startpos.x + 0.5), y = (size_t)(startpos.y + 0.5);
	double lastx = startpos.x - 0.5, lasty = startpos.y - 0.5;
	NextFrontier nf = NextFrontier();
	Node * refCur;
	Node * refNext;
	map = world->getMap();

	
	//more variables
	//If already at goal, ezpz
	if (x == destinationPos.x && y == destinationPos.y) {
		nodes.push_back(&referenceNodes[destinationPos.y][destinationPos.x]);
		Debugger::print("Set0: " + strh::toString(i1) + ",\t" + strh::toString(i2) + ",\t" + strh::toString(i3) + ",\t" +
			strh::toString(i4) + ",\t" + strh::toString(i5) + ",\t" + strh::toString(i6) + ",\t" +
			strh::toString(i7) + ",\t" + strh::toString(i8) + ",\t" + strh::toString(i9) + "\n");
		return Path(nodes);
	}

	//Flag start position as explored, dont start path from here.
	flags[y][x] = true;

	//Debugger::print("initialized node checkpoints:" + toString(nodes[0].checkpoints.size()) + "\n");
	findUnexploredSurroundingPaths(Vector2ST(x, y), &nf); //Start out with the nodes around the start position;
	for (int i = 0; i < 4; i++) {
		if (nf.inFrontier[i]) nodes.push_back(&referenceNodes[nf.nodes[i].pos.y][nf.nodes[i].pos.x]);
	}
	nodesSize = nodes.size();

	for (size_t i = 0; i < nodesSize; i++) {//Flag initial nodes
		flags[nodes[i]->pos.y][nodes[i]->pos.x] = true;
		if (nodes[i]->pos.x ==destinationPos.x && nodes[i]->pos.y == destinationPos.y) { //If one is the destination, return start and that as vector
			vector<Node*> n;
			n.push_back(&referenceNodes[(int)startpos.y][(int)startpos.x]); //Start pos
			n.push_back(nodes[i]);
			Debugger::print("Set1: " + strh::toString(i1) + ",\t" + strh::toString(i2) + ",\t" + strh::toString(i3) + ",\t" +
				strh::toString(i4) + ",\t" + strh::toString(i5) + ",\t" + strh::toString(i6) + ",\t" +
				strh::toString(i7) + ",\t" + strh::toString(i8) + ",\t" + strh::toString(i9) + "\n");
			return Path(n);
		}
		i2++;
	}

	int counter = 0;
	
	while (nodesSize > 0 && counter < 10000) { //while there are still more frontiers to explore
		i3++;
		current = findShortestNode(nodes); //Select the shortest frontier
		//Debugger::print("Current node has size: " + toString(current.size) + " and position: " + toString(current.pos.x) + ",\t" + toString(current.pos.y) + " and length " + toString(current.checkpoints.size()) + "\n");
		findUnexploredSurroundingPaths(current->pos, &nf); //Find best directions to advance

		for (size_t i = 0; i < 4; i++) {
			if(nf.inFrontier[i]){
				i4++;
				//Manage the reference node
				refCur = &referenceNodes[current->pos.y][current->pos.x];
				refNext = &referenceNodes[nf.nodes[i].pos.y][nf.nodes[i].pos.x];
				//Set where it was explored from
				refNext->previousNode = refCur;
				//Set total size
				refNext->size = world->getMap()->getTravelWeightAt(refNext->pos.x, refNext->pos.y) + refCur->size;
				//Create node on frontier
				nodes.push_back(refNext);
				//Flag the position as explored
				flags[refNext->pos.y][refNext->pos.x] = true;

				//Debugger::print("Returned node has size: " + toString(next.size) + " and position: " + toString(next.pos.x) + ",\t" + toString(next.pos.y) + " and length " + toString(next.checkpoints.size()) + "\n");
				//Debugger::print("Checkpoints are: ");
				//for (int i = 0; i < next.checkpoints.size(); i++) {
				//	Debugger::print(toString(next.checkpoints[i].pos.x) + ",\t" + toString(next.checkpoints[i].pos.y) + " ");
				//}
				//Debugger::print("\n"); 

				//If the explored node is the goal
				if (refNext->pos.x == destinationPos.x && refNext->pos.y == destinationPos.y) {
					//create and return a vector containing the explored path
					int i = 1;
					vector<Node> path;
					path.insert(path.begin(), *refNext);
					do {
						refNext = refNext->previousNode; //If the path before that node does not have a path, stop
						path.insert(path.begin(), *refNext); //Insert current node
						i++;
					} while (refNext->previousNode != NULL);
					//Make sure he doesnt stray
					path.insert(path.begin(), Node(x, y, 0));
					//return the path
					Debugger::print("Set3: " + strh::toString(i1) + ",\t" + strh::toString(i2) + ",\t" + strh::toString(i3) + ",\t" + 
											strh::toString(i4) + ",\t" + strh::toString(i5) + ",\t" + strh::toString(i6) + ",\t" + 
											strh::toString(i7) + ",\t" + strh::toString(i8) + ",\t" + strh::toString(i9) + "\n");
					return Path(path);
				}
			}
		}
		//Node now explored, erase
		nodes.erase(nodes.begin() + stTemp0);
		nodesSize = nodes.size();
		counter++;
	}

	//If there is no path
	vector<Node> start;
	start.push_back(Node((size_t)startpos.x, (size_t)startpos.y, 0));
	Debugger::print("Set2: " + strh::toString(i1) + ",\t" + strh::toString(i2) + ",\t" + strh::toString(i3) + ",\t" +
		strh::toString(i4) + ",\t" + strh::toString(i5) + ",\t" + strh::toString(i6) + ",\t" +
		strh::toString(i7) + ",\t" + strh::toString(i8) + ",\t" + strh::toString(i9) + "\n");
	return Path(start);
	
}

Node* Navigator::findShortestNode(vector<Node*> nodes) {
	//Look through all nodes and return the smallest. If it is the frontier node list, also note cIndex, to mark it for erasion if empty frontier
	//(When searching for 'current')
	//Compare to first
	stTemp0 = 0;
	dtemp0 = nodes[0]->size;

	for (size_t i = 1; i < nodesSize; i++) {//for each other element in frontier
		if (nodes[i]->size < dtemp0) { //If size is smaller, set as prefered
			dtemp0 = nodes[i]->size;
			stTemp0 = i;
			i6++;
		}
		i5++;
	}
	i7++;
	return nodes[stTemp0];
}

void Navigator::findUnexploredSurroundingPaths(Vector2ST pos, NextFrontier* nf) {
	//Find the surrounding paths, that are not flagged as explored
	i8++;
	vector<Node> steps;
	if (pos.x >= 1u //index x cant be below zero
		&& !flags[pos.y][pos.x - 1]//Flag must be false
		&& map->getBorderDataAt(pos.x, pos.y, pos.x - 1, pos.y, 3)) { //Border must be crossable

		nf->nodes[0].pos.x = pos.x - 1;
		nf->nodes[0].pos.y = pos.y;
		nf->nodes[0].size = map->getTravelWeightAt(pos.x - 1, pos.y);
		nf->inFrontier[0] = true;
		i9++;
	}
	else nf->inFrontier[0] = false;

	if (pos.x + 1u < map->sizex //Index x cant be above the size of the y array at pos
		&& !flags[pos.y][pos.x + 1] //Flag must be false
		&& world->getMap()->getBorderDataAt(pos.x, pos.y, pos.x + 1, pos.y, 1)) { //Border must be crossable

		nf->nodes[1].pos.x = pos.x + 1;
		nf->nodes[1].pos.y = pos.y;
		nf->nodes[1].size = map->getTravelWeightAt(pos.x + 1, pos.y);
		nf->inFrontier[1] = true; 
		i9++;
	}
	else nf->inFrontier[1] = false;

	if (pos.y >= 1 //Index y cant be above 0, and index cant be above the size of the y array at pos
		&& !flags[pos.y - 1][pos.x] //Flag must be false
		&& map->getBorderDataAt(pos.x,pos.y, pos.x, pos.y - 1, 0)) { //Border must be crossable

		nf->nodes[2].pos.x = pos.x;
		nf->nodes[2].pos.y = pos.y - 1;
		nf->nodes[2].size = map->getTravelWeightAt(pos.x, pos.y - 1);
		nf->inFrontier[2] = true; 
		i9++;
	}
	else nf->inFrontier[2] = false;

	if (pos.y + 1u < map->sizey //Pos y must be below the amount of y's and index x cant be above the size of the y array at pos
		&& !flags[pos.y + 1][pos.x] //Flag must be false
		&& map->getBorderDataAt(pos.x, pos.y, pos.x, pos.y + 1, 2)) { //Border must be crossable

		nf->nodes[3].pos.x = pos.x;
		nf->nodes[3].pos.y = pos.y + 1;
		nf->nodes[3].size = map->getTravelWeightAt(pos.x, pos.y + 1);
		nf->inFrontier[3] = true;
		i9++;
	}
	else nf->inFrontier[3] = false;

	int x = 0;

}

