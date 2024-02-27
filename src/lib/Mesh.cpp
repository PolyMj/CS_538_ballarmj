#include "Mesh.hpp"

namespace potato {
    // Compute bounds for single face
    void computeBounds(vector<Vert> &vertices, Face &face, BoundBoxf &box, bool startBox) {
        
        // If restarting box, start to first value index
        if(startBox) {
            box.start = vertices.at(face.indices.at(0)).pos;
            box.end = vertices.at(face.indices.at(0)).pos;
        }
        
        // Loop through remaining indices, picking the smallest/largest         
        for(int i = 0; i < face.indices.size(); i++) {
            Vec3f v = vertices.at(face.indices.at(i)).pos;
            box.start = minV(box.start, v);
            box.end = maxV(box.end, v);
        }
    };

    // Compute bounds for ENTIRE mesh
    void computeBounds(PolyMesh *mesh, BoundBoxf &box, bool startBox) {
        
        // For each face...
        vector<Face> faces = mesh->getFaces();
        vector<Vert> vertices = mesh->getVertices();

        for(int i = 0; i < faces.size(); i++) {
            computeBounds(vertices, faces.at(i), box, startBox);
            startBox = false;
        }
    };
};