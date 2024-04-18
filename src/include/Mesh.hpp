#pragma once 
 
#define _USE_MATH_DEFINES 
#include <cmath> 
#include <vector> 
#include <iostream> 
#include "Vector.hpp" 
#include "Object3D.hpp" 
#include "BoundBox.hpp" 
using namespace std; 
 
namespace potato { 
 
    struct Vert { 
        Vec3f pos {}; 
        // Union is used because several other programs refer to this attribute as "color",
        // but I specifically want diffuse color
        union {Vec4f color {}, diffuse; }; // [0,1] 
        Vec4f specular;

 
        Vert operator+(const Vert &other) const { 
            return { 
                pos + other.pos, 
                color + other.color 
            }; 
        }; 
 
        Vert operator-(const Vert &other) const { 
            return { 
                pos - other.pos, 
                color - other.color 
            }; 
        }; 
 
        Vert operator*(const Vert &other) const { 
            return { 
                pos * other.pos, 
                color * other.color 
            }; 
        }; 
 
        Vert operator*(const float &w) const { 
            return { 
                pos * w, 
                color * w 
            }; 
        }; 
    }; 
 
    struct Face { 
        vector<unsigned int> indices {}; 
    }; 
 
    class PolyMesh : public Object3D { 
    protected: 
        vector<Vert> vertices {}; 
        vector<Face> faces {}; 
    public: 
        PolyMesh() : Object3D() {}; 
        PolyMesh(PolyMesh *pm) {
            Object3D();
            vertices = vector<Vert>(pm->vertices);
            faces = vector<Face>(pm->faces);
        };
        virtual ~PolyMesh() {}; 

        vector<Vert>& getVertices() { return vertices; }; 
        vector<Face>& getFaces() { return faces; }; 
    }; 
 
    // Compute bounds for single face 
    void computeBounds(vector<Vert> &vertices, Face &face, BoundBoxf &box, bool startBox=true); 
 
    // Compute bounds for ENTIRE mesh 
    void computeBounds(PolyMesh *mesh, BoundBoxf &box, bool startBox=true); 
}; 
