#include "Meshd.hpp" 

#define LINE_SIZE 128

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

    PolyMeshd* loadOBJTriangleMesh(string filename, Vec4d diff_color) {
		PolyMeshd *pm = new PolyMeshd();
		
		// Open .obj file
		FILE *file = fopen(filename.c_str(), "r");
		if (file == NULL) {
			cerr << "ERROR in loadOBJModel: file \"" << filename << "\" does not exist" << endl;
			return NULL;
		}
		
		// For each line in .obj
		char line[LINE_SIZE];
		while (fgets(line, LINE_SIZE, file) != NULL) {
			// Create string version of the line
			stringstream line_stream = stringstream(line);
			string identifier;
			
			// Grab and check the "identifier"
			line_stream >> identifier;
			if (identifier == "v") { // Is a vertex (x,y,z,w)
				Vertd v = Vertd();
				try {
					line_stream >> v.pos.x >> v.pos.y >> v.pos.z;
					v.color = diff_color;
				}
				catch (exception e) {
					cerr << "WARNING in loadOBJModel: Vertex coordinates were improperly loaded: " 
						<< e.what() << endl;
				}
				pm->getVertices().push_back(v);
			}
			else if (identifier == "f") { // Is a face - List of vertex indices starting at 1
				Faced f = Faced();
				try {
					// For each whitespace-separated chunk
					string chunk;
					while (line_stream >> chunk) {
						// Get vertex index (starting at 1)
						int index = 0;
						for (char c : chunk) {
							if (c < '0' || c > '9') {
								break;
							}
							else {
								index *= 10;
								index += c - '0';
							}
						}

						// If index is valid, subtract 1 and add to the face indicies
						if (index > 0) {
							f.indices.push_back(index-1);
						}
						else {
							cerr << "WARNING in loadOBJModel: Invalid face data: \""
								<< chunk << "\" from line \"" << line << "\"" << endl;
						}
					}
					pm->getFaces().push_back(f);
				}
				catch (exception e) {
					cerr << "ERROR in loadOBJModel: " << e.what() << endl;
				}
			}
			else {
				// We don't care yet
			}
		}
		
		cout << "\nFilename: \"" << filename << "\"" << endl;
		cout << " - Vertices = " << pm->getVertices().size() << endl;

		fclose(file);
		return pm;
	}
}; 