#pragma once

#include <iostream>
#include <vector>
#include "glm/glm.hpp"
using namespace std;

// Struct for holding vertex data
struct Vertex {
	glm::vec3 position;
	glm::vec2 texcoords;
};

// Struct for holding mesh data
struct Mesh {
	vector<Vertex> vertices;
	vector<unsigned int> indices;
};
