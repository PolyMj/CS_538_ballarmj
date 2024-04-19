#pragma once 
 
#define _USE_MATH_DEFINES 
#include <cmath> 
#include <vector> 
#include <iostream> 
#include "Vector.hpp"  
#include "BoundBox.hpp"
using namespace std; 
 
namespace potato { 
 
    struct Vertd { 
        Vec3d pos {}; 
        // Union is used because several other programs refer to this attribute as "color",
        // but I specifically want diffuse color
        union {Vec4d color {}, diffuse; }; // [0,1] 
        Vec4d specular;
        Vec3d normal;

 
        Vertd operator+(const Vertd &other) const { 
            return { 
                pos + other.pos, 
                color + other.color 
            }; 
        }; 
 
        Vertd operator-(const Vertd &other) const { 
            return { 
                pos - other.pos, 
                color - other.color 
            }; 
        }; 
 
        Vertd operator*(const Vertd &other) const { 
            return { 
                pos * other.pos, 
                color * other.color 
            }; 
        }; 
 
        Vertd operator*(const float &w) const { 
            return { 
                pos * w, 
                color * w 
            }; 
        }; 
    }; 
 
    struct Faced { 
        vector<unsigned int> indices {}; 
    }; 
 
    class PolyMeshd { 
    protected: 
        vector<Vertd> vertices {}; 
        vector<Faced> faces {}; 
    public: 
        PolyMeshd() {}; 
        PolyMeshd(PolyMeshd *pm) {
            vertices = vector<Vertd>(pm->vertices);
            faces = vector<Faced>(pm->faces);
        };
        virtual ~PolyMeshd() {}; 

        vector<Vertd>& getVertices() { return vertices; }; 
        vector<Faced>& getFaces() { return faces; }; 
    }; 
 
    // Compute bounds for single face 
    void computeBounds(vector<Vertd> &vertices, Faced &face, BoundBoxd &box, bool startBox=true); 
 
    // Compute bounds for ENTIRE mesh 
    void computeBounds(PolyMeshd *mesh, BoundBoxd &box, bool startBox=true); 
}; 
