#include "Debugger.h"
#include "StringHelper.h"
#include "MapWindow.h"
#include "Navigator.h"
#include <time.h>
#include <chrono>

using namespace std;

Path::Path() {}
int Path::pathLength() {
	return nodes[nodes.size() - 1].size;
}



Navigator::Navigator() {
}
Navigator::Navigator(MapWindow * mapWindow) {
	world = mapWindow;
}

Path Navigator::findPath(Vector2 startpos, Vector2 destinationPos) {
	//initialization
	//Define variables
	clock_t t = clock();
	clock_t dt = 0;
	long cps = CLOCKS_PER_SEC;
	vector<Node> nodes;//all the frontier nodes with more paths to explore
	int x = (int)(startpos.x + 0.5), y = (int)(startpos.y + 0.5);
	double lastx = startpos.x - 0.5, lasty = startpos.y - 0.5;

	//If already at goal, ezpz
	if (x == (int)(destinationPos.x) && y == (int)(destinationPos.y)) {
		nodes.push_back(Node((int)destinationPos.x, (int)destinationPos.y, 0));
		return Path(nodes);
	}
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
	//Flag start position as explored, dont start path from here.
	flags[y][x] = true;

	//Debugger::print("initialized node checkpoints:" + toString(nodes[0].checkpoints.size()) + "\n");
	nodes = findUnexploredSurroundingPaths(Vector2(x, y)); //Start out with the nodes around the start position;
	for (size_t i = 0; i < nodes.size(); i++) {//Flag initial nodes
		flags[(int)(nodes[i].pos.y)][(int)(nodes[i].pos.x)] = true;
		if (nodes[i].pos.x == (int)destinationPos.x && nodes[i].pos.y == (int)destinationPos.y) { //If one is the destination, return as vector
			vector<Node> n;
			n.push_back(nodes[i]);
			//Make sure he doesnt stray
			n.insert(n.begin(), Node(x, y, 0));
			return Path(n);
		}
	}

	long aaloopTime = 0;
	long aashortestNodeTime = 0;
	long aaunexploredPathTime = 0;
	int counter = 0;

	while (nodes.size() > 0 && counter < 10000) { //while there are still more frontiers to explore
		auto loopTimerBegin = std::chrono::high_resolution_clock::now();
		auto shortestNodeTimerBegin = std::chrono::high_resolution_clock::now();
		Node current = findShortestNode(nodes, true); //Select the shortest frontier
		auto shortestNodeTimerEnd = std::chrono::high_resolution_clock::now();
		//Debugger::print("Current node has size: " + toString(current.size) + " and position: " + toString(current.pos.x) + "," + toString(current.pos.y) + " and length " + toString(current.checkpoints.size()) + "\n");
		auto unexploredPathBegin = std::chrono::high_resolution_clock::now();
		vector<Node> next = findUnexploredSurroundingPaths(current.pos); //Find best direction to advance
		auto unexploredPathEnd = std::chrono::high_resolution_clock::now();
		for (size_t i = 0; i < next.size(); i++) {
			//Manage the reference node
			Node * refCur = &referenceNodes[(size_t)current.pos.y][(size_t)current.pos.x];
			Node * refNext = &referenceNodes[(size_t)next[i].pos.y][(size_t)next[i].pos.x];
			//Set where it was explored from
			refNext->previousNode = refCur;
			//Set total size
			refNext->size = world->getMap()->getTravelWeightAt((int)(refNext->pos.x), (int)(refNext->pos.y)) + refCur->size;
			//Create node on frontier
			nodes.push_back(refNext->clone());
			//Flag the position as explored
			flags[(int)(next[i].pos.y)][(int)(next[i].pos.x)] = true;

			//Debugger::print("Returned node has size: " + toString(next.size) + " and position: " + toString(next.pos.x) + "," + toString(next.pos.y) + " and length " + toString(next.checkpoints.size()) + "\n");
			//Debugger::print("Checkpoints are: ");
			//for (int i = 0; i < next.checkpoints.size(); i++) {
			//	Debugger::print(toString(next.checkpoints[i].pos.x) + "," + toString(next.checkpoints[i].pos.y) + " ");
			//}
			//Debugger::print("\n"); 

			//If the explored node is the goal
			if (next[i].pos.x == (int)destinationPos.x && next[i].pos.y == (int)destinationPos.y) {
				//create a vector containing the explored path
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
			//Create reference node
		}
		//Node now explored, erase
		nodes.erase(nodes.begin() + cIndex);
		//Go to new frontier

		//counter++;

		auto loopTimerEnd = std::chrono::high_resolution_clock::now();
		aaloopTime = std::chrono::duration_cast<std::chrono::nanoseconds>(loopTimerEnd - loopTimerBegin).count();
		aaunexploredPathTime = std::chrono::duration_cast<std::chrono::nanoseconds>(unexploredPathEnd - unexploredPathBegin).count();
		aashortestNodeTime = std::chrono::duration_cast<std::chrono::nanoseconds>(shortestNodeTimerEnd - shortestNodeTimerBegin).count();
	}

	//If there is no path
	vector<Node> start;
	start.push_back(Node(startpos.x, startpos.y, 0));
	return Path(start);

}

Node Navigator::findShortestNode(vector<Node> nodes, bool frontierNodes) {
	//Look through all nodes and return the smallest. If it is the frontier node list, also note cIndex, to mark it for erasion if empty frontier
	//(When searching for 'current')
	//Compare to first
	Node smallestN = nodes[0];
	if (frontierNodes) cIndex = 0;
	long tPerNode;
	int nodeSize();
	tPerNode = nodes.size();
	for (size_t i = 1; i < nodes.size(); i++) {//for each other element in frontier
		auto tPerNodeB = std::chrono::high_resolution_clock::now();
		if (nodes[i].size < smallestN.size) { //If size is smaller, set as prefered
			smallestN = nodes[i];
			if (frontierNodes) cIndex = i;
		}
		auto tPerNodeE = std::chrono::high_resolution_clock::now();
		tPerNode = std::chrono::duration_cast<std::chrono::nanoseconds>(tPerNodeE - tPerNodeB).count();
		int q = 0;
	}
	return smallestN;

}

vector<Node> Navigator::findUnexploredSurroundingPaths(Vector2 pos) {
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
	if (pos.x - 1 >= 0 //index x cant be below zero
		&& !flags[(int)(pos.y)][(int)(pos.x - 1)]//Flag must be false
		&& world->getMap()->getBorderDataAt(pos, (int)(pos.x - 1), (int)pos.y, 3)) { //Border must be crossable
		auto tPathsOutsideAE = std::chrono::high_resolution_clock::now();
		atPathsOutsideA = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsOutsideAE - tPathsOutsideAB).count();

		auto tPathsInsideAB = std::chrono::high_resolution_clock::now();
		steps.push_back(Node((int)(pos.x - 1), (int)pos.y, world->getMap()->getTravelWeightAt((int)(pos.x - 1), (int)pos.y))); //Add node with position and step weight
		auto tPathsInsideAE = std::chrono::high_resolution_clock::now();
		atPathsInsideA = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsInsideAE - tPathsInsideAB).count();
	}
	tPathsOutsideAB = std::chrono::high_resolution_clock::now();
	if (pos.x + 1 < flags[(int)(pos.y)].size() //Index x cant be above the size of the y array at pos
		&& !flags[(int)(pos.y)][(int)(pos.x + 1)] //Flag must be false
		/*&& world->getMap()->getBorderDataAt(pos, (int)(pos.x + 1), (int)pos.y, 1)*/) { //Border must be crossable
		auto tPathsOutsideAE = std::chrono::high_resolution_clock::now();
		atPathsOutsideB = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsOutsideAE - tPathsOutsideAB).count();

		auto tPathsInsideAB = std::chrono::high_resolution_clock::now();
		steps.push_back(Node((int)(pos.x + 1), (int)pos.y, world->getMap()->getTravelWeightAt((int)(pos.x + 1), (int)pos.y))); //Add node with position and step weight
		auto tPathsInsideAE = std::chrono::high_resolution_clock::now();
		atPathsInsideB = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsInsideAE - tPathsInsideAB).count();

	}
	tPathsOutsideAB = std::chrono::high_resolution_clock::now();
	if (pos.y - 1 >= 0 && pos.x < flags[(int)(pos.y - 1)].size() //Index y cant be above 0, and index cant be above the size of the y array at pos
		&& !flags[(int)(pos.y - 1)][(int)(pos.x)] //Flag must be false
		&& world->getMap()->getBorderDataAt(pos, (int)(pos.x), (int)pos.y - 1, 0)) { //Border must be crossable
		auto tPathsOutsideAE = std::chrono::high_resolution_clock::now();
		atPathsOutsideC = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsOutsideAE - tPathsOutsideAB).count();

		auto tPathsInsideAB = std::chrono::high_resolution_clock::now();
		steps.push_back(Node((int)pos.x, (int)(pos.y - 1), world->getMap()->getTravelWeightAt((int)pos.x, (int)(pos.y - 1)))); //Add node with position and step weight
		auto tPathsInsideAE = std::chrono::high_resolution_clock::now();
		atPathsInsideC = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsInsideAE - tPathsInsideAB).count();
	}
	tPathsOutsideAB = std::chrono::high_resolution_clock::now();
	if (pos.y + 1 < flags.size() && pos.x < flags[(int)(pos.y + 1)].size() //Pos y must be below the amount of y's and index x cant be above the size of the y array at pos
		&& !flags[(int)(pos.y + 1)][(int)(pos.x)] //Flag must be false
		&& world->getMap()->getBorderDataAt(pos, (int)(pos.x), (int)pos.y + 1, 2)) { //Border must be crossable
		auto tPathsOutsideAE = std::chrono::high_resolution_clock::now();
		atPathsOutsideD = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsOutsideAE - tPathsOutsideAB).count();
		Node n = Node((int)pos.x, (int)(pos.y + 1), 0);
		auto tPathsInsideAB = std::chrono::high_resolution_clock::now();
		n.size = world->getMap()->getTravelWeightAt((int)pos.x, (int)(pos.y + 1));
		auto tPathsInsideAE = std::chrono::high_resolution_clock::now();
		steps.push_back(n); //Add node with position and step weight
		atPathsInsideD = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsInsideAE - tPathsInsideAB).count();
	}
	auto tPathsE = std::chrono::high_resolution_clock::now();
	atPaths = std::chrono::duration_cast<std::chrono::nanoseconds>(tPathsE - tPathsB).count();
	return steps;
}
Node Navigator::findShortestSurroundingPaths(Node n) {
	//find unexplored surrounding paths
	vector<Node> surroundingPaths = findUnexploredSurroundingPaths(n.pos);

	//Find the shortest of them
	Node shortest = findShortestNode(surroundingPaths);
	//Debugger::print("Shortest path has size: " + toString((int)(shortest.size)) + "\n");
	return shortest;
}