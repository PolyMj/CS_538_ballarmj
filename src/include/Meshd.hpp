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
        Vec3d normal;
    }; 
 
    class PolyMeshd { 
    protected: 
        vector<Vertd> vertices {}; 
        vector<Faced> faces {}; 
        BoundBoxd bb;
    public: 
        PolyMeshd() {}; 
        PolyMeshd(PolyMeshd *pm) {
            vertices = vector<Vertd>(pm->vertices);
            faces = vector<Faced>(pm->faces);
        };
        virtual ~PolyMeshd() {}; 

        vector<Vertd>& getVertices() { return vertices; }; 
        vector<Faced>& getFaces() { return faces; }; 
        BoundBoxd getBB() { return bb; };

        // Only sets face normals, not vertices
        void computeNormals() {
            for (int i = 0; i < faces.size(); i++) {
                computeNormal(faces[i]);
            }
        };

        void computeNormal(Faced &face) {
            Vertd v0 = vertices.at(face.indices.at(0));
            Vertd v1 = vertices.at(face.indices.at(1));
            Vertd v2 = vertices.at(face.indices.at(2));

            Vec3d AB = v1.pos - v0.pos;
            Vec3d AC = v2.pos - v0.pos;
            face.normal = AB.cross(AC).normalize();
        }

        void addTriangleFace(int A, int B, int C) {
            Faced face = Faced();
            face.indices.push_back(A);
            face.indices.push_back(B);
            face.indices.push_back(C);
            computeNormal(face);
            faces.push_back(face);
        }

        void addTriangleFace(Vertd A, Vertd B, Vertd C) {
            int starti = vertices.size();
            vertices.push_back(A);
            vertices.push_back(B);
            vertices.push_back(C);

            Faced face = Faced();
            for (int i = starti; i < vertices.size(); i++) {
                face.indices.push_back(i);
            }
            computeNormal(face);
            faces.push_back(face);
        }

        void computeBounds() {
            if (vertices.size() <= 0) return;
            bb.start = vertices.at(0).pos;
            bb.end = bb.start;

            for (int i = 1; i < vertices.size(); i++) {
                Vec3d v = vertices.at(i).pos;
                bb.start = minV(bb.start, v);
                bb.end = maxV(bb.end, v);
            }
        }
    }; 
 
    // Compute bounds for single face 
    void computeBounds(vector<Vertd> &vertices, Faced &face, BoundBoxd &box, bool startBox=true); 
 
    // Compute bounds for ENTIRE mesh 
    void computeBounds(PolyMeshd *mesh, BoundBoxd &box, bool startBox=true); 
}; 
