#include "Meshd.hpp" 
 
namespace potato { 
    // Compute bounds for single face 
    void computeBounds(vector<Vertd> &vertices, Faced &face, BoundBoxd &box, bool startBox) {
 
        // If restarting box, start to first value index 
        if(startBox) { 
            box.start = vertices.at(face.indices.at(0)).pos; 
            box.end = vertices.at(face.indices.at(0)).pos; 
        } 
 
        // Loop through remaining indices, picking the smallest/largest 
        for(int i = 0; i < face.indices.size(); i++) { 
            Vec3d v = vertices.at(face.indices.at(i)).pos; 
            box.start = minV(box.start, v); 
            box.end = maxV(box.end, v); 
        } 
    }; 
 
    // Compute bounds for ENTIRE mesh 
    void computeBounds(PolyMeshd *mesh, BoundBoxd &box, bool startBox) {
 
        // For each face... 
        vector<Faced> faces = mesh->getFaces(); 
        vector<Vertd> vertices = mesh->getVertices(); 
 
        for(int i = 0; i < faces.size(); i++) { 
            computeBounds(vertices, faces.at(i), box, startBox); 
            startBox = false; 
        } 
    }; 
}; 