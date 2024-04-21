#include "PotatoRaytracerEngine.hpp"

#define HIT_BB_COLOR Vec4d(0.1f, 0.1f, 0.1f, 1.0f)
#define X_AXIS Vec4d(1.0f, 0.0f, 0.0f, 0.0f)
#define Y_AXIS Vec4d(0.0f, 1.0f, 0.0f, 0.0f)
#define Z_AXIS Vec4d(0.0f, 0.0f, 1.0f, 0.0f)
#define W_AXIS Vec4d(0.0f, 0.0f, 0.0f, 1.0f)

// Load models
PotatoRaytracerEngine::PotatoRaytracerEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) {
	double Z = -3.6f;

	PolyMeshd *model = new PolyMeshd();
	model = loadOBJTriangleMesh("sampleModels/teapot.obj");
	model->transform(Mat4d(X_AXIS, Y_AXIS, Z_AXIS, Vec4d(1.5, -1.5, -20.0, 1.0)));
	model->debugRecolor();
	meshes.push_back(model);
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
			last_vert.color = HIT_BB_COLOR; // For debugging
			for (int i = 0; i < mesh->getFaces().size(); i++) {
				FaceData face = mesh->getFaceData(i);
				
				double cld_t = ray.collide(face.v1.pos, face.normal);
				if (cld_t >= 0) {
					Vec3d pos = ray.posFromT(cld_t);
					Vec3d bary = bary3D(pos, face.v0.pos, face.v1.pos, face.v2.pos, face.normal);
					if (isInside(bary)) {
						Vertd current_vert;
						current_vert.color = interpolateBary(bary, face.v0.color, face.v1.color, face.v2.color);
						current_vert.pos = interpolateBary(bary, face.v0.pos, face.v1.pos, face.v2.pos);
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

	return Vec3f(last_vert.color);
}