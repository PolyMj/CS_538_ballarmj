#include "PotatoRaytracerEngine.hpp"

// Load models
PotatoRaytracerEngine::PotatoRaytracerEngine(int windowWidth, int windowHeight) : PotatoRaytracerEngine(windowWidth, windowHeight) {
	
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
void PotatoRaytracerEngine::renderToDrawBuffer(Image<Vec3f> * drawBuffer) {
	// For each ray starting from camera
		// raycast()
}

// Returns the color from the given raycast
Vec3f PotatoRaytracerEngine::raycast(Ray ray) {
	// Repeat [MAX_BOUNCES] times
		// If ray collides with an object
			// Ray is now its reflection { ray.reflect(normal) }
			// (Potentially) Check and add ambient/diffuse lighting in some way
		// Else
			// Break
	// Return ray.color
}