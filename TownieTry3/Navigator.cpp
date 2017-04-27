#include "Debugger.h"
#include "StringHelper.h"
#include "MapWindow.h"
#include "Navigator.h"
#include <time.h>
#include <chrono>

using namespace std;

Path::Path() {}
double Path::pathLength() {
	return nodes[nodes.size() - 1].size;
}



Navigator::Navigator() {
}
Navigator::Navigator(MapWindow * mapWindow) {
	world = mapWindow;
}


Path Navigator::findPath(Vector2D startpos, Vector2ST destinationPos) {
	//initialization
	//Define variables
	vector<Node*> nodes;//all the frontier nodes with more paths to explore
	size_t x = (size_t)(startpos.x + 0.5), y = (size_t)(startpos.y + 0.5);
	double lastx = startpos.x - 0.5, lasty = startpos.y - 0.5;
	NextFrontier nf = NextFrontier();
	Node * refCur;
	Node * refNext;
	map = world->getMap();

	
	//more variables
	vector<vector<Node>> referenceNodes; //For storing where the node was reached from
	vector<vector<bool>> temp;
	flags = temp;
	for (size_t y = 0; y < world->getMap()->tiles.size(); y++) {
		vector<bool> tempb;
		vector<Node> tempn;
		flags.push_back(tempb);
		referenceNodes.push_back(tempn);
		for (size_t x = 0; x < world->getMap()->tiles[y].size(); x++) {
			flags[y].push_back(false);
			referenceNodes[y].push_back(Node(x, y, 0));
		}
	}
	//If already at goal, ezpz
	if (x == destinationPos.x && y == destinationPos.y) {
		nodes.push_back(&referenceNodes[destinationPos.y][destinationPos.x]);
		return Path(nodes);
	}

	//Flag start position as explored, dont start path from here.
	flags[y][x] = true;

	//Debugger::print("initialized node checkpoints:" + toString(nodes[0].checkpoints.size()) + "\n");
	findUnexploredSurroundingPaths(Vector2I(x, y), &nf); //Start out with the nodes around the start position;
	for (int i = 0; i < 4; i++) {
		if (nf.inFrontier[i]) nodes.push_back(&referenceNodes[nf.nodes[i].pos.y][nf.nodes[i].pos.x]);
	}

	for (size_t i = 0; i < nodes.size(); i++) {//Flag initial nodes
		flags[nodes[i]->pos.y][nodes[i]->pos.x] = true;
		if (nodes[i]->pos.x ==destinationPos.x && nodes[i]->pos.y == destinationPos.y) { //If one is the destination, return start and that as vector
			vector<Node*> n;
			n.push_back(&referenceNodes[(int)startpos.y][(int)startpos.x]); //Start pos
			n.push_back(nodes[i]);
			return Path(n);
		}
	}

	long aaloopTime = 0;
	long aashortestNodeTime = 0;
	long aaunexploredPathTime = 0;
	int counter = 0;
	
	while (nodes.size() > 0 && counter < 100) { //while there are still more frontiers to explore
		auto loopTimerBegin = std::chrono::high_resolution_clock::now();
		auto shortestNodeTimerBegin = std::chrono::high_resolution_clock::now();
		current = findShortestNode(nodes); //Select the shortest frontier
		auto shortestNodeTimerEnd = std::chrono::high_resolution_clock::now();
		//Debugger::print("Current node has size: " + toString(current.size) + " and position: " + toString(current.pos.x) + "," + toString(current.pos.y) + " and length " + toString(current.checkpoints.size()) + "\n");
		auto unexploredPathBegin = std::chrono::high_resolution_clock::now();
		findUnexploredSurroundingPaths(current->pos, &nf); //Find best directions to advance
		
		auto unexploredPathEnd = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < 4; i++) {
			if(nf.inFrontier[i]){
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

				//Debugger::print("Returned node has size: " + toString(next.size) + " and position: " + toString(next.pos.x) + "," + toString(next.pos.y) + " and length " + toString(next.checkpoints.size()) + "\n");
				//Debugger::print("Checkpoints are: ");
				//for (int i = 0; i < next.checkpoints.size(); i++) {
				//	Debugger::print(toString(next.checkpoints[i].pos.x) + "," + toString(next.checkpoints[i].pos.y) + " ");
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
					return Path(path);
				}
			}
		}
		//Node now explored, erase
		nodes.erase(nodes.begin() + itemp0);
		counter++;
		auto loopTimerEnd = std::chrono::high_resolution_clock::now();
		aaloopTime = std::chrono::duration_cast<std::chrono::nanoseconds>(loopTimerEnd - loopTimerBegin).count();
		aaunexploredPathTime = std::chrono::duration_cast<std::chrono::nanoseconds>(unexploredPathEnd - unexploredPathBegin).count();
		aashortestNodeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(shortestNodeTimerEnd - shortestNodeTimerBegin).count();
	}

	//If there is no path
	vector<Node> start;
	start.push_back(Node((size_t)startpos.x, (size_t)startpos.y, 0));
	return Path(start);
	
}

Node* Navigator::findShortestNode(vector<Node*> nodes) {
	//Look through all nodes and return the smallest. If it is the frontier node list, also note cIndex, to mark it for erasion if empty frontier
	//(When searching for 'current')
	//Compare to first
	itemp0 = 0;
	dtemp0 = nodes[0]->size;

	for (size_t i = 1; i < nodes.size(); i++) {//for each other element in frontier
		if (nodes[i]->size < dtemp0) { //If size is smaller, set as prefered
			dtemp0 = nodes[i]->size;
			itemp0 = i;
		}
	}
	return nodes[itemp0];
}

void Navigator::findUnexploredSurroundingPaths(Vector2ST pos, NextFrontier* nf) {
	//Find the surrounding paths, that are not flagged as explored
	long atPaths;
	long atPathsInsideA = 0;
	long atPathsInsideB = 0;
	long atPathsInsideC = 0;
	long atPathsInsideD = 0;
	long atPathsOutsideA = 0;
	long atPathsOutsideB = 0;
	long atPathsOutsideC = 0;
	long atPathsOutsideD = 0;

	auto tPathsB = std::chrono::high_resolution_clock::now();
	vector<Node> steps;
	auto tPathsOutsideAB = std::chrono::high_resolution_clock::now();
	if (pos.x >= 1u //index x cant be below zero
		&& !flags[pos.y][pos.x - 1]//Flag must be false
		&& world->getMap()->getBorderDataAt(pos.x, pos.y, pos.x - 1, pos.y, 3)) { //Border must be crossable
		auto tPathsOutsideAE = std::chrono::high_resolution_clock::now();
		atPathsOutsideA = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsOutsideAE - tPathsOutsideAB).count();

		auto tPathsInsideAB = std::chrono::high_resolution_clock::now();
		nf->nodes[0].pos.x = pos.x - 1;
		nf->nodes[0].pos.y = pos.y;
		nf->nodes[0].size = world->getMap()->getTravelWeightAt(pos.x - 1, pos.y);
		nf->inFrontier[0] = true;
		auto tPathsInsideAE = std::chrono::high_resolution_clock::now();
		atPathsInsideA = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsInsideAE - tPathsInsideAB).count();
	}
	else nf->inFrontier[0] = false;

	tPathsOutsideAB = std::chrono::high_resolution_clock::now();
	if (pos.x + 1u < map->sizex //Index x cant be above the size of the y array at pos
		&& !flags[pos.y][pos.x + 1] //Flag must be false
		&& world->getMap()->getBorderDataAt(pos.x, pos.y, pos.x + 1, pos.y, 1)) { //Border must be crossable
		auto tPathsOutsideAE = std::chrono::high_resolution_clock::now();
		atPathsOutsideB = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsOutsideAE - tPathsOutsideAB).count();

		auto tPathsInsideAB = std::chrono::high_resolution_clock::now();
		nf->nodes[1].pos.x = pos.x + 1;
		nf->nodes[1].pos.y = pos.y;
		nf->nodes[1].size = world->getMap()->getTravelWeightAt(pos.x + 1, pos.y);
		nf->inFrontier[1] = true; 
		auto tPathsInsideAE = std::chrono::high_resolution_clock::now();
		atPathsInsideB = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsInsideAE - tPathsInsideAB).count();
	}
	else nf->inFrontier[1] = false;

	tPathsOutsideAB = std::chrono::high_resolution_clock::now();
	if (pos.y >= 1 //Index y cant be above 0, and index cant be above the size of the y array at pos
		&& !flags[pos.y - 1][pos.x] //Flag must be false
		&& world->getMap()->getBorderDataAt(pos.x,pos.y, pos.x, pos.y - 1, 0)) { //Border must be crossable
		auto tPathsOutsideAE = std::chrono::high_resolution_clock::now();
		atPathsOutsideC = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsOutsideAE - tPathsOutsideAB).count();

		auto tPathsInsideAB = std::chrono::high_resolution_clock::now();
		nf->nodes[2].pos.x = pos.x;
		nf->nodes[2].pos.y = pos.y - 1;
		nf->nodes[2].size = world->getMap()->getTravelWeightAt(pos.x, pos.y - 1);
		nf->inFrontier[2] = true; 
		auto tPathsInsideAE = std::chrono::high_resolution_clock::now();
		atPathsInsideC = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsInsideAE - tPathsInsideAB).count();
	}
	else nf->inFrontier[2] = false;

	tPathsOutsideAB = std::chrono::high_resolution_clock::now();
	if (pos.y + 1u < map->sizey //Pos y must be below the amount of y's and index x cant be above the size of the y array at pos
		&& !flags[pos.y + 1][pos.x] //Flag must be false
		&& map->getBorderDataAt(pos.x, pos.y, pos.x, pos.y + 1, 2)) { //Border must be crossable
		auto tPathsOutsideAE = std::chrono::high_resolution_clock::now();
		atPathsOutsideD = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsOutsideAE - tPathsOutsideAB).count();

		auto tPathsInsideAB = std::chrono::high_resolution_clock::now();
		nf->nodes[3].pos.x = pos.x;
		nf->nodes[3].pos.y = pos.y + 1;
		nf->nodes[3].size = world->getMap()->getTravelWeightAt(pos.x, pos.y + 1);
		nf->inFrontier[3] = true;
		auto tPathsInsideAE = std::chrono::high_resolution_clock::now();
		atPathsInsideD = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsInsideAE - tPathsInsideAB).count();
		int i = 0;
	}
	else nf->inFrontier[3] = false;

	auto tPathsE = std::chrono::high_resolution_clock::now();
	atPaths = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsE - tPathsB).count();
	int x = 0;


}