#include "PotatoRaytracerEngine.hpp"

// Load models
PotatoRaytracerEngine::PotatoRaytracerEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) {
	double Z = -2.4f;
	// Testing PolyMeshd
	PolyMeshd *mesh = new PolyMeshd();
	Vertd v1 = Vertd();
	v1.pos = Vec3d(-1.5f, -1.0f, Z);
	v1.color = Vec4d(0.0f, 1.0f, 1.0f, 1.0f);
	Vertd v2 = Vertd();
	v2.pos = Vec3d(1.5f, -1.0f, Z);
	v2.color = Vec4d(1.0f, 0.0f, 1.0f, 1.0f);
	Vertd v3 = Vertd();
	v3.pos = Vec3d(1.5f, 1.0f, Z);
	v3.color = Vec4d(1.0f, 1.0f, 0.0f, 1.0f);

	mesh->addTriangleFace(v1, v2, v3);
	mesh->computeBounds();
	meshes.push_back(mesh);
}

// Delete/clear all data
PotatoRaytracerEngine::~PotatoRaytracerEngine() {
    for(int i = 0; i < allMeshes.size(); i++) { 
        delete allMeshes.at(i); 
    } 
    allMeshes.clear(); 
    renderMeshes.clear();
 
    allFragments.clear(); 

}

void PotatoRaytracerEngine::processAllGeomtry() {
	// Get view transformation matrix using cameraPos and cameraLookat
		// Might not even be necessary
	// For each mesh
		// processGeometryOneMesh()
	
}

void PotatoRaytracerEngine::processGeomtryOneMesh(Mat4f modelMat, Mat4f viewMat) {
	// Look at PotatoForwardEngine.cpp
		// Will be similar, but modifcation will be needed
}

// Draw everything to draw buffer; called once per drawing loop iteration
void PotatoRaytracerEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) {
	// For each ray starting from camera
		// raycast()
	allThreads.clear();
	drawBuffer->clear();
	for (int y = 0; y < windowHeight; y += ROWS_PER_THREAD) {
		if (y+ROWS_PER_THREAD >= windowHeight) {
			allThreads.push_back( new thread(&PotatoRaytracerEngine::drawRows, this, drawBuffer, y, windowHeight-y) );
		}
		else {
			allThreads.push_back( new thread(&PotatoRaytracerEngine::drawRows, this, drawBuffer, y, ROWS_PER_THREAD) );
		}
	}

	for (thread* thread : allThreads) {
		thread->join();
		delete thread;
	}
}

void PotatoRaytracerEngine::drawRows(Image<Vec3f> *drawBuffer, int start_y, int count) {
	for (int y = start_y; y < count+start_y; y++) {
		for (int x = 0; x < windowWidth; x++) {
			drawBuffer->setPixel(x,y, raycast(Ray(windowWidth, windowHeight, x, y)));
		}
	}
}

// Returns the color from the given raycast
Vec3f PotatoRaytracerEngine::raycast(Ray ray) {
	Vec3f color;
	// Repeat [MAX_BOUNCES] times
		// If ray collides with an object
			// Ray is now its reflection { ray.reflect(normal) }
			// (Potentially) Check and add ambient/diffuse lighting in some way
		// Else
			// Break

	Vertd last_vert;
	bool hasBeenACollide = false;

	for (int i = 0; i < meshes.size(); i++) {
		PolyMeshd *mesh = meshes.at(i);
		
		if (ray.intersectsBBox(mesh->getBB())) {
			for (int i = 0; i < mesh->getFaces().size(); i++) {
				Faced face = mesh->getFaces().at(i);
				Vertd v0 = mesh->getVertices().at(face.indices.at(0));
				Vertd v1 = mesh->getVertices().at(face.indices.at(1));
				Vertd v2 = mesh->getVertices().at(face.indices.at(2));
				
				double cld_t = ray.collide(v1.pos, face.normal);
				if (cld_t >= 0) {
					Vec3d pos = ray.posFromT(cld_t);
					Vec3d bary = bary3D(pos, v0.pos, v1.pos, v2.pos, face.normal);
					if (isInside(bary)) {
						Vertd current_vert;
						current_vert.color = interpolateBary(bary, v0.color, v1.color, v2.color);
						current_vert.pos = interpolateBary(bary, v0.pos, v1.pos, v2.pos);
						current_vert.normal = face.normal;

						if (hasBeenACollide) {
							// Store closer one in last vert
							if (last_vert.pos.z > current_vert.pos.z) {
								last_vert = current_vert;
							}
						}
						else {
							last_vert = current_vert;
							hasBeenACollide = true;
						}
					}
				}
			}
		}
	}

	// // Clipping
	// float color_clip = 2.0f;
	// for (int i = 0; i < 3; i++) {
	// 	color[i] = std::max(0.0f, color[i]);
	// 	color[i] = color[i] / (color_clip + color[i]);
	// }

	return last_vert.color;
}