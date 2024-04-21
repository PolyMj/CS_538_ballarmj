#include "PotatoRaytracerEngine.hpp"

#define HIT_BB_COLOR Vec4d(0.1f, 0.1f, 0.1f, 1.0f)
#define X_AXIS Vec4d(1.0f, 0.0f, 0.0f, 0.0f)
#define Y_AXIS Vec4d(0.0f, 1.0f, 0.0f, 0.0f)
#define Z_AXIS Vec4d(0.0f, 0.0f, 1.0f, 0.0f)
#define W_AXIS Vec4d(0.0f, 0.0f, 0.0f, 1.0f)

// Load models
PotatoRaytracerEngine::PotatoRaytracerEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) {
	
	
	// I think the matrix multiplication is implemented incorrectly
	// I need to take the transpose of whatever I do for it do work
	// Will fix later
	Mat4d modelMat = (translate<double>(2.4, -3.0, -19) * uniformScale(0.9)).transpose();
	PolyMeshd *c1 = new PolyMeshd();
	c1 = loadOBJTriangleMesh("sampleModels/cube.obj");
	c1->blendNormals = false;
	c1->transform(modelMat);
	c1->uniformRecolor(Vec3d(1.0, 0.5, 0.5));
	meshes.push_back(c1);

	modelMat = (translate<double>(2.1, -0.4, -3.6) * uniformScale(0.8)).transpose();
	PolyMeshd *c2 = new PolyMeshd();
	c2 = loadOBJTriangleMesh("sampleModels/cube.obj");
	c2->blendNormals = false;
	c2->transform(modelMat);
	c2->uniformRecolor(Vec3d(1.0, 1.0, 0.4));
	meshes.push_back(c2);


	modelMat = (translate<double>(-7.8, -1.5, 4.0) * uniformScale(1.8)).transpose();
	PolyMeshd *tp = new PolyMeshd();
	tp = loadOBJTriangleMesh("sampleModels/cube.obj");
	tp->blendNormals = false;
	tp->transform(modelMat);
	tp->uniformRecolor(Vec3d(0.3, 1.0, 0.3));
	meshes.push_back(tp);


	modelMat =  (translate<double>(-3.0, -1.0, -8) * scale<double>(1.6, 1.6, 4.0)).transpose();
	PolyMeshd *m1 = new PolyMeshd();
	m1 = loadOBJTriangleMesh("sampleModels/cube.obj");
	m1->blendNormals = false;
	m1->transform(modelMat);
	m1->uniformRecolor(Vec3d(0.7, 0.7, 1.0));
	meshes.push_back(m1);

	modelMat =  (translate<double>(-3.0, -1.0, -40) * scale<double>(15, 15, 0.1)).transpose();
	PolyMeshd *m2 = new PolyMeshd();
	m2 = loadOBJTriangleMesh("sampleModels/cube.obj");
	m2->blendNormals = false;
	m2->transform(modelMat);
	m2->uniformRecolor(Vec3d(1.0, 0.85, 0.7));
	meshes.push_back(m2);
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

Vec3f PotatoRaytracerEngine::raycast(Ray ray) {
	
	Vertd col_vert;
	for (int i = 0; i < MAX_BOUNCES; i++) {
		if (collideRay(ray, col_vert)) {
			ray.reflectSelf(col_vert);
		}
		else {
			break;
		}
	}
	
	// Scale the light based on direction of light "source"
	double scalar = (1.0 + MIN_SKY_LIGHT + ray.direction.dot(lightDirection)) / (2.0 + 2*MIN_SKY_LIGHT);
	return Vec3f(ray.color * scalar);
}


struct BBCollideData {
	int meshIndex;
	double near_t;
	double far_t;
};

// Returns the color from the given raycast
bool PotatoRaytracerEngine::collideRay(Ray ray, Vertd &col_vert) {
	
	vector<BBCollideData> bb_collides = {};
	// For each mesh
	for (int i = 0; i < meshes.size(); i++) {
		PolyMeshd* mesh = meshes.at(i);
		// Get collison data
		double t1, t2;
		if (!ray.intersectsBBox(mesh->getBB(), t1, t2))
			continue; // Skip if doesn't intersect

		// Ensure that t1 is the first intersection
		if (t2 < t1)
			swap(t1, t2);

		// If there was only one intersection, the ray started insdie the bounding box
		if (t1 < 0)
			t1 = 0; // Set initial "intersection" to ray start
		
		// Put all data into a struct
		BBCollideData new_collide = {
			i,
			t1,
			t2
		};
		
		// If list of collides is empty, add first one
		if (!bb_collides.size()) {
			bb_collides.push_back(new_collide);
			continue;
		}

		bool added = false;
		for (int i = 0; i < bb_collides.size(); i++) {
			// Insert new collide to correct location (sorted by near_t)
			if (bb_collides.at(i).near_t > new_collide.near_t) {
				bb_collides.insert(bb_collides.begin()+i, new_collide);
				added = true;
				break; // new_collide already inserted, no need to continue
			}
		}

		// If it hasn't been added, add it to the end of the list
		if (!added)
			bb_collides.push_back(new_collide);
	}


	Vertd last_vert;
	double last_cld_t = -1;
	bool hasBeenACollide = false;

	// For each bounding box collision
	for (BBCollideData bb_cld : bb_collides) {
		// Get the corresponding mesh
		PolyMeshd *mesh = meshes.at(bb_cld.meshIndex);
		
		// Get the bounding box of the ray's intersection with the mesh's bounding box
		BoundBoxd bb = BoundBoxd(ray.posFromT(bb_cld.near_t), ray.posFromT(bb_cld.far_t));

		// If the closest possible collision is still further than another found collision
		if (hasBeenACollide && min(bb_cld.near_t, bb_cld.far_t) > last_cld_t)
			continue;
		
		// For each face
		for (int fi = 0; fi < mesh->getFaces().size(); fi++) {
			// Get the data for the face (each vertex, face normal, etc.)
			FaceData face = mesh->getFaceData(fi);
			
			// If the face doesn't inersect with the ray's bounding box
			if (face.getESCLCode(bb))
				continue; // Skip
			
			// If the ray must travel backwards to collide with face-plane
			double cld_t = ray.collide(face.v1.pos, face.normal);
			if (cld_t < 0)
				continue;

			// If there was another closer collision
			if (hasBeenACollide && cld_t > last_cld_t)
				continue;
			
			// Get barycentric coordinates of collision
			Vec3d pos = ray.posFromT(cld_t);
			Vec3d bary = bary3D(pos, face.v0.pos, face.v1.pos, face.v2.pos, face.normal);
			
			// If did not colide
			if (!isInside(bary))
				continue; // Skip to next face

			last_cld_t = cld_t;
			
			Vertd current_vert = face.interpolateFaceBary(bary, false);
			current_vert.pos = pos;
				

			last_vert = current_vert;
			hasBeenACollide = true;
		}
	}

	
	col_vert = last_vert;
	return hasBeenACollide;
}