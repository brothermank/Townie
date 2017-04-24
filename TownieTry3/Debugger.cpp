#include "Debugger.h"

ofstream out;

Debugger::Debugger() {

}

void Debugger::print(string line) {
	out << line.c_str();
}

void initDebug() {
	out.open("Debug.txt");
}

