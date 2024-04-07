#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Mesh.hpp"
using namespace std;

#define LINE_SIZE 64

namespace potato {
	PolyMesh* loadOBJModel(string filename, Vec4f color = Vec4f(0.0f, 0.4f, 0.4f, 0.5f));
};