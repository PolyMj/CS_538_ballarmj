#include "Rasterize.hpp"

namespace potato {    
    // Generates mesh for testing  
    PolyMesh* generateTestFan(const Vec3f &center, float radius, int triangleCnt) {
        // Generate vertices
        PolyMesh *m = new PolyMesh();
        double angleInc = double(2.0f*M_PI/(triangleCnt*2.0f));
        float colorAngleOffset = float(2.0f*M_PI/3.0f);

        Vert centerVert;
        centerVert.pos = center;
        centerVert.color = Vec4f(1.0f,1.0f,1.0f,1.0f);
        m->getVertices().push_back(centerVert);

        int vcnt = triangleCnt*2;

        for(int i = 0; i < vcnt; i++) {
            float angle = float(angleInc*i);
            Vert v;
            v.pos = Vec3f(radius*cos(angle), radius*sin(angle), 0.0f) + center;
            float r = std::max(0.0f, cos(angle));
            float g = std::max(0.0f, cos(angle - colorAngleOffset));
            float b = std::max(0.0f, cos(angle - 2.0f*colorAngleOffset));
            v.color = Vec4f(r,g,b,1.0f);
            m->getVertices().push_back(v);            
        }

        // Generate triangles
        for(int i = 0; i < vcnt; i+=2) {
            Face f;
            f.indices.push_back(0);
            f.indices.push_back(i+1);
            f.indices.push_back(i+2);
            m->getFaces().push_back(f);
        }

        return m;    
    };

    // Draws lines for mesh
    void drawLines(PolyMesh *mesh, vector<Fragment> &fragList, bool wireframe) {     
        vector<Vert> vertices = mesh->getVertices(); 
        vector<Face> faces = mesh->getFaces();

        void (*drawLine) (Vert&, Vert&, vector<Fragment>&, bool) {};
        if(LINE_ALGORITHM == LINE_DDA) {
            drawLine = &drawLineDDA;
        }
        else if(LINE_ALGORITHM == LINE_MID) {
            drawLine = &drawLineMid;
        }
        else {
            throw std::invalid_argument("Bad line drawing algorithm!");
        }
        
        for(int i = 0; i < faces.size(); i++) {
            // For each line...
            for(int k = 0; k < faces.at(i).indices.size(); k++) {
                int firstIndex = faces.at(i).indices.at(k);
                int secondIndex = faces.at(i).indices.at((k+1)%faces.at(i).indices.size());
                drawLine(   vertices.at(firstIndex),
                            vertices.at(secondIndex),
                            fragList, wireframe);
            }
        }
    };

    // Fills triangles for mesh
    void fillTriangles(PolyMesh *mesh, vector<Fragment> &allFragments) {
        vector<Vert> vertices = mesh->getVertices(); 
        vector<Face> faces = mesh->getFaces();
        for(int i = 0; i < faces.size(); i++) {
            fillTriangle(vertices, faces.at(i), allFragments);            
        }
    };

    // Computes interpolated fragment
    Fragment computeFragment(Vert &vA, Vert &vB, Vert &vC, Vec3f &bc) {
        Vert FV = (vA*bc.x) + (vB*bc.y) + (vC*bc.z);
        Fragment frag;
        frag.pos = roundV(FV.pos);        
        frag.color = FV.color;
        return frag;
    };

    // TODO ///////////////////////////////////////////////////////////////////

    void drawLineDDA(Vert &startVert, Vert &endVert, vector<Fragment> &fragList, bool wireframe) {
        // Convert to nearest integer locations
        
        // Get differences
        
        // Calculate steps and increments
        
        // Get starting coordinates 
        
        // Get starting color and increments
        // (Switch to white if wireframe is true)
        
        // Draw first pixel (really add fragment)
        
        // For each step...  
            // Add fragment to list      
    };

    void drawLineMid(Vert &startVert, Vert &endVert, vector<Fragment> &fragList, bool wireframe) {
        // Convert to nearest integer locations
        
        // Get differences
        
        // Is change in Y greater?
        
        // Start starting and ending color
        
        // Is X going in the negative direction?
        
        // Set starting y
        
        // Is Y going in the negative direction
        
        // Calculate implicit line
        
        // Calculate starting decision variable value
        
        // Get starting color and increments
        
        // For each step
        
            // Add fragment
            
            // Increment color
            
            // Check decision
            
    };

    void fillTriangle(vector<Vert> &vertices, Face &face, vector<Fragment> &allFragments) {
        // Compute bounding box
        
        // Convert to integer bounding box
        
        // You may assume we have triangles only

        // Get barycentric data
                
        // Loop through each position        
                // Calculate barycentric weights
                
                // If within bounds...(greater than 0)
                
                    // Calculate fragment
                    
                    // Add to list                    
               
    };
};
