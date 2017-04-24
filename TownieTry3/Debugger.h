#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string>
using namespace std;

extern ofstream out;

class Debugger {
public:
	Debugger();
	static void print(string s);

private:
};

void initDebug();