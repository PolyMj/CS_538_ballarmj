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
            v.pos.x = std::round(v.pos.x);
            v.pos.y = std::round(v.pos.y);
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
    void fillTriangles(PolyMesh *mesh, vector<Fragment> &fragList) {
        vector<Vert> vertices = mesh->getVertices(); 
        vector<Face> faces = mesh->getFaces();
        for(int i = 0; i < faces.size(); i++) {
            fillTriangle(vertices, faces.at(i), fragList);            
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
		Vec3i start = Vec3i(roundV(startVert.pos));
		Vec3i end = Vec3i(roundV(endVert.pos));
        
        // Get differences
		int dx = end.x - start.x;
		int dy = end.y - start.y;
        
        // Calculate steps and increments
		int steps = std::max(abs(dx), abs(dy));
		float xInc = float(dx) / float(steps);
		float yInc = float(dy) / float(steps);
        
        // Get starting coordinates 
		float x = float(start.x);
		float y = float(start.y);
        
        // Get starting color and increments
        // (Switch to white if wireframe is true)
		Vec4f colorInc;
		Vec4f currColor;
		if (wireframe) {
			currColor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
			colorInc = Vec4f(0,0,0,0);
		}
		else {
			currColor = startVert.color;
			colorInc = (endVert.color - startVert.color) / steps;
		}
        
        // Draw first pixel (really add fragment)
		Fragment firstFrag = Fragment();
		firstFrag.pos = Vec3i(x, y, 0);
		firstFrag.color = currColor;
		fragList.push_back(firstFrag);

        // For each step...  
		for (int i = 0; i < steps; i++) {
			// Increment x, y, and color
			x += xInc;
			y += yInc;
			currColor = currColor + colorInc;

			Fragment newFrag = Fragment();
			newFrag.color = currColor;
			newFrag.pos = Vec3i(round(x), round(y), 0);

			// Add fragment to list
			fragList.push_back(newFrag);
		}
    };

    void drawLineMid(Vert &startVert, Vert &endVert, vector<Fragment> &fragList, bool wireframe) {
        // Convert to nearest integer locations
		Vec3i start = roundV(startVert.pos);
		Vec3i end = roundV(endVert.pos);
        
        // Get differences
		int dx = end.x - start.x;
		int dy = end.y - start.y;
        
        // Is change in Y greater?
		bool xySwapped = abs(dy) > abs(dx);
		if (xySwapped) {
			std::swap(dx, dy);
			std::swap(start.x, start.y);
			std::swap(end.x, end.y);
		}

        // Start starting and ending color
		Vec4f startColor = startVert.color;
		Vec4f endColor = endVert.color;
        
        // Is X going in the negative direction?
		if (dx < 0) {
			std::swap(start, end);
			std::swap(startColor, endColor);
			dx = -dx;
			dy = -dy;
		}
        
        // Set starting y
		float y = start.y;
        
        // Is Y going in the negative direction
		int yInc = 1;
		if (dy < 0) {
			yInc = -1;
			dy = -dy;
			std::swap(start.y, end.y); // This works with or without this swap, not sure why
		}
        
        // Calculate implicit line
		ImplicitLine<float> line = ImplicitLine<float>(start, end);
        
        // Calculate starting decision variable value
		float d = line.eval(start.x+1.0f, start.y+0.5f);
		
		int steps = end.x - start.x;
        
        // Get starting color and increments
		Vec4f currColor;
		Vec4f colorInc;
		if (wireframe) {
			currColor = Vec4f(1.0f, 1.0f, 1.0f, 1.0f);
			colorInc = Vec4f(0.0f, 0.0f, 0.0f, 0.0f);
		}
		else {
			currColor = startColor;
			colorInc = (endColor - startColor) / steps;
		}
        
        // For each step
		for (int x = start.x; x <= end.x; x++) {
            Fragment f = Fragment();
			if (xySwapped)
				f.pos = Vec3i(y,x,0);
			else
				f.pos = Vec3i(x,y,0);
			
			f.color = currColor;
			
			// Add fragment
			fragList.push_back(f);
            
            // Increment color
			currColor = currColor + colorInc;
            
            // Check decision
			if (d < 0) {
				y += yInc;
				d += (dx-dy);
			}
			else {
				d -= dy;
			}
		}
    };

    void fillTriangle(vector<Vert> &vertices, Face &face, vector<Fragment> &fragList) {
        // Compute bounding box
		BoundBoxf bbf;
		computeBounds(vertices, face, bbf, true);
        
        // Convert to integer bounding box
		BoundBoxi bbi = convertBoundBox(bbf);
        
        // You may assume we have triangles only (thank you)
		Vert vA = vertices.at(face.indices.at(0));
		Vert vB = vertices.at(face.indices.at(1));
		Vert vC = vertices.at(face.indices.at(2));

        // Get barycentric data
		BaryData barryD = BaryData(vA.pos, vB.pos, vC.pos); // Bro thinks he's the flash
                
        // Loop through each position
		for (int x = bbi.start.x; x <= bbi.end.x; x++) {
			for (int y = bbi.start.y; y <= bbi.end.y; y++) {
				// Calculate barrycentric weights
				Vec3f barryW = barycentric(barryD, float(x), float(y));

				// If within bounds, calculate and add fragment
				if (barryW.x > 0 && barryW.y > 0 && barryW.z > 0) {
					Fragment f = computeFragment(vA, vB, vC, barryW);
					fragList.push_back(f);
				}
			}
		}                        
    };

	void drawCircleMid(Vert &centerVert, float r, vector<Fragment> &fragList) {
		float d = 5.0f/4.0f - r;
		int x = 0;
		int y = (int)r;

		while (x <= y) {
			addOctantFragments(centerVert, x, y, fragList);
			x++;
			
			if (d < 0) {
				d += 2*x + 1;
			}
			else {
				y--;
				d += 2*x - 2*y;
			}
		}
	}

	void addOctantFragments(Vert &centerVert, float xoff, float yoff, vector<Fragment> &fragList) {
			for (int i = 0; i < 8; i++) {
				float x = xoff;
				float y = yoff;

				if (i & 1)
					y = -y;
				if (i & 2)
					x = -x;
				if (i & 4)
					std::swap(x,y);

				Fragment f;
				f.pos = Vec3i(x,y,0) + roundV(centerVert.pos);
				f.color = centerVert.color;

				fragList.push_back(f);
			}
			

	}
};
