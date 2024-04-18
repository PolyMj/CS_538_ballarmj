#include "PotatoRaytracerEngine.hpp"

// Load models
PotatoRaytracerEngine::PotatoRaytracerEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) {
	float Z = -10.0f;
	tri.A.pos = Vec3f(-1.5f, -1.0f, Z);
	tri.B.pos = Vec3f(1.5f, -1.0f, Z);
	tri.C.pos = Vec3f(1.5f, 1.0f, Z);
	tri.computeNormal();

	tri.A.color = Vec4f(0.0f, 1.0f, 1.0f, 1.0f);
	tri.B.color = Vec4f(1.0f, 0.0f, 1.0f, 1.0f);
	tri.C.color = Vec4f(1.0f, 1.0f, 0.0f, 1.0f);
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
	drawBuffer->clear();
	for (int x = 0; x < windowWidth; x++) {
		Ray ray;
		for (int y = 0; y < windowHeight; y++) {
			ray = Ray(windowWidth, windowHeight, x, y);
			drawBuffer->setPixel(x, y, raycast(ray));
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

	float cld_t = ray.collide(tri.A.pos, tri.normal);

	if (cld_t >= 0) {
		Vec3f pos = ray.posFromT(cld_t);
		Vec3f bary = bary3D(pos, tri.A.pos, tri.B.pos, tri.C.pos, tri.normal);
		if (isInside(bary)) {
			color = tri.A.color*bary.x + tri.B.color*bary.y + tri.C.color*bary.z;
		}
	}

	// // Clipping
	// float color_clip = 2.0f;
	// for (int i = 0; i < 3; i++) {
	// 	color[i] = std::max(0.0f, color[i]);
	// 	color[i] = color[i] / (color_clip + color[i]);
	// }

	return color;
}