#pragma once 
 
#define _USE_MATH_DEFINES 
#include <cmath> 
#include <vector> 
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Vector.hpp"  
#include "BoundBox.hpp"
#include "Matrix.hpp"
#include "Bary.hpp"
#include "Settings.hpp"
using namespace std; 
 
namespace potato { 
 
    struct Vertd { 
        Vec3d pos {}; 
        // Union is used because several other programs refer to this attribute as "color",
        // but I specifically want diffuse color
        union {Vec3d color {}, diffuse; }; // [0,1] 
        Vec3d specular;
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
 
    struct FaceData {
        Vertd v0, v1, v2;
        Vec3d normal;
        bool interp_normals = true;

        int getESCLCode(BoundBoxd bb) {
            return (
                bb.getECSLCode(v0.pos) &
                bb.getECSLCode(v1.pos) &
                bb.getECSLCode(v2.pos)
            );
        };

        Vertd interpolateFaceBary(Vec3d bary, bool interp_pos = true) {
            Vertd interp;
            if (interp_pos) interp.pos = interpolateBary(bary, v0.pos, v1.pos, v2.pos);
            
            // Decide wether to use face normal or interpolated normal
            if (interp_normals)
                interp.normal = interpolateBary(bary, v0.normal, v1.normal, v2.normal);
            else
                interp.normal = normal;
            
            // Will need to add specular & diffuse later
            interp.color = interpolateBary(bary, v0.color, v1.color, v2.color);

            return interp;
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
        bool blendNormals = true;

        PolyMeshd() {}; 
        PolyMeshd(PolyMeshd *pm) {
            vertices = vector<Vertd>(pm->vertices);
            faces = vector<Faced>(pm->faces);
        };
        virtual ~PolyMeshd() {}; 

        vector<Vertd>& getVertices() { return vertices; }; 
        vector<Faced>& getFaces() { return faces; }; 
        BoundBoxd getBB() { return bb; };

        FaceData getFaceData(int findex) {
            Faced f = faces.at(findex);
            return FaceData {
                vertices.at(f.indices.at(0)),
                vertices.at(f.indices.at(1)),
                vertices.at(f.indices.at(2)),
                f.normal,
                blendNormals
            };
        };

        // Only sets face normals, not vertices
        void computeNormals() {
            // Reset vertex normals
            if (blendNormals) {
                for (int i = 0; i < vertices.size(); i++) {
                    vertices.at(i).normal = Vec3d(0,0,0);
                }
            }
            
            // Compute face normals and add them to each adjacent vertex
            for (int i = 0; i < faces.size(); i++) {
                computeNormal(faces[i]);
            }

            // Renormalize vertex normals
            if (blendNormals) {
                for (int i = 0; i < vertices.size(); i++) {
                    vertices.at(i).normal = vertices.at(i).normal.normalize();
                }
            }
        };

        void computeNormal(Faced &face) {
            Vertd& v0 = vertices.at(face.indices.at(0));
            Vertd& v1 = vertices.at(face.indices.at(1));
            Vertd& v2 = vertices.at(face.indices.at(2));

            Vec3d AB = v1.pos - v0.pos;
            Vec3d AC = v2.pos - v0.pos;
            face.normal = AB.cross(AC).normalize();
            v0.normal = v0.normal + face.normal;
            v1.normal = v1.normal + face.normal;
            v2.normal = v2.normal + face.normal;
        };

        void addTriangleFace(int A, int B, int C) {
            Faced face = Faced();
            face.indices.push_back(A);
            face.indices.push_back(B);
            face.indices.push_back(C);
            computeNormal(face);
            faces.push_back(face);
        };

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
        };

        void computeBounds() {
            if (vertices.size() <= 0) return;
            bb.start = vertices.at(0).pos;
            bb.end = bb.start;

            for (int i = 1; i < vertices.size(); i++) {
                Vec3d v = vertices.at(i).pos;
                bb.start = minV(bb.start, v);
                bb.end = maxV(bb.end, v);
            }

            Vec3d offset = (bb.end-bb.start) * BASICALLY_ZERO;
            bb.end = bb.end + offset;
            bb.start = bb.start - offset;
        };

        void transform(Mat4d mat) {
            for (int i = 0; i < vertices.size(); i++) {
                Vertd& v = vertices.at(i);
                Vec4d pos = mat * Vec4d(v.pos, 1.0f);
                v.pos = Vec3d(pos) / pos.w;
            }
            computeNormals(); // Could use a normal transform but this is easier for now
            computeBounds();
        };

        void uniformRecolor(Vec3d color) {
            for (int i = 0; i < vertices.size(); i++) {
                vertices.at(i).color = color;
            }
        };

        void debugRecolor() {
            for (int i = 0; i < vertices.size(); i++) {
                Vec3d pos = vertices.at(i).pos;
                vertices.at(i).color = Vec3d(
                    abs(pos.x) / (abs(pos.x) + 1),
                    abs(pos.y) / (abs(pos.y) + 1),
                    abs(pos.z) / (abs(pos.z) + 1)
                );
            }
        };
    }; 
 
    // Compute bounds for single face 
    void computeBounds(vector<Vertd> &vertices, Faced &face, BoundBoxd &box, bool startBox=true); 
 
    // Compute bounds for ENTIRE mesh 
    void computeBounds(PolyMeshd *mesh, BoundBoxd &box, bool startBox=true); 

    // Load a .obj model
    PolyMeshd* loadOBJTriangleMesh(string filename);
}; 
