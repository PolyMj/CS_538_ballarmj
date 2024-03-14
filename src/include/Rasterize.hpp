#pragma once 
 
#define _USE_MATH_DEFINES 
#include <cmath> 
#include <vector> 
#include <iostream> 
#include "Vector.hpp" 
#include "Object3D.hpp" 
#include "Fragment.hpp" 
#include "BoundBox.hpp" 
#include "Mesh.hpp" 
#include "Bary.hpp" 
#include "Settings.hpp" 
using namespace std; 
 
namespace potato { 
    // Already provided 
    PolyMesh* generateTestFan(const Vec3f &center, float radius, int triangleCnt); 
    void drawLines(PolyMesh *mesh, vector<Fragment> &fragList, bool wireframe); 
    void fillTriangles(PolyMesh *mesh, vector<Fragment> &fragList); 
    Fragment computeFragment(Vert &vA, Vert &vB, Vert &vC, Vec3f &bc); 
 
    // TODO FOR ASSIGNMENT 
    void drawLineDDA(Vert &startVert, Vert &endVert, vector<Fragment> &fragList, bool wireframe); 
    void drawLineMid(Vert &startVert, Vert &endVert, vector<Fragment> &fragList, bool wireframe); 
    void fillTriangle(vector<Vert> &vertices, Face &face, vector<Fragment> &fragList); 
 
}; 
