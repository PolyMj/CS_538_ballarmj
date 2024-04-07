#include "OBJModel.hpp"
using namespace std;

#define LINE_SIZE 64

namespace potato {
	// Set the color of a vertex based on its position
	void setColor(Vert *v) {
		v->color.r = (v->pos.x + 1.2f) / 2.4f;
		v->color.g = (v->pos.y + 1.2f) / 2.4f;
		v->color.b = (v->pos.z + 1.2f) / 2.4f;
	}
	
	// Load a .obj file into a PolyMesh
	PolyMesh* loadOBJModel(string filename) {
		PolyMesh *pm = new PolyMesh();
		
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
				Vert v = Vert();
				try {
					line_stream >> v.pos.x >> v.pos.y >> v.pos.z;
					setColor(&v);
				}
				catch (exception e) {
					cerr << "WARNING in loadOBJModel: Vertex coordinates were improperly loaded: " 
						<< e.what() << endl;
				}
				pm->getVertices().push_back(v);
			}
			else if (identifier == "f") { // Is a face - List of vertex indices starting at 1
				Face f = Face();
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