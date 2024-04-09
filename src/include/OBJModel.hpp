#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Mesh.hpp"
using namespace std;

#define LINE_SIZE 64

namespace potato {
	PolyMesh* loadOBJModel(string filename);
};