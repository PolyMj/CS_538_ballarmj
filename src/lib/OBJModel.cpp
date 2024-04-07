#include "OBJModel.hpp"
using namespace std;

#define LINE_SIZE 64

namespace potato {
	PolyMesh* loadOBJModel(string filename, Vec4f color) {
		PolyMesh *pm = new PolyMesh();
		
		FILE *file = fopen(filename.c_str(), "r");
		if (file == NULL) {
			cerr << "ERROR in loadOBJModel: file \"" << filename << "\" does not exist" << endl;
			return NULL;
		}
		
		char line[LINE_SIZE];
		while (fgets(line, LINE_SIZE, file) != NULL) {
			stringstream line_stream = stringstream(line);
			string identifier;
			
			line_stream >> identifier;
			if (identifier == "v") { // Vertex - In x,y,z,w
				Vert v = Vert();
				try {
					line_stream >> v.pos.x >> v.pos.y >> v.pos.z;
					v.color = color;
				}
				catch (exception e) {
					cerr << "WARNING in loadOBJModel: Vertex coordinates were improperly loaded: " 
						<< e.what() << endl;
				}
				pm->getVertices().push_back(v);
			}
			else if (identifier == "f") { // Face - List of vertex indices
				Face f = Face();
				try {
					string chunk;
					while (line_stream >> chunk) {
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

						if (index > 0) {
							f.indices.push_back(index-1);
						}
						else {
							cerr << "WARNING in loadOBJModel: Invalid face data: \"" << 
								chunk << "\" from line \"" << line << "\"" << endl;
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

		cout << "Vertices = " << pm->getVertices().size() << endl;
	
		fclose(file);
		return pm;
	}
};