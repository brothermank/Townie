#pragma once
#include "Vector.h"
#include "Debugger.h"
#include "StringHelper.h"

class Rectangle {
public:
	int x, y, w, h;

	Rectangle() {
		Debugger::print("Creating default rect\n");
	}
	Rectangle(int x, int y, int w, int h) : x(x), y(y), w(w), h(h) {
		Debugger::print("Created rectangle with pos " + strh::toString(x) + "," + strh::toString(y) + "  size " + strh::toString(w) + "," + strh::toString(h) + "\n");
	}

};