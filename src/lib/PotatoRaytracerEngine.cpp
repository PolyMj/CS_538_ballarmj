#include "PotatoRaytracerEngine.hpp"

#define HIT_BB_COLOR Vec4d(0.1f, 0.1f, 0.1f, 1.0f)
#define X_AXIS Vec4d(1.0f, 0.0f, 0.0f, 0.0f)
#define Y_AXIS Vec4d(0.0f, 1.0f, 0.0f, 0.0f)
#define Z_AXIS Vec4d(0.0f, 0.0f, 1.0f, 0.0f)
#define W_AXIS Vec4d(0.0f, 0.0f, 0.0f, 1.0f)

#define DEG_90 (90.0*3.14/180.0)

// Load models and place them into meshes
PotatoRaytracerEngine::PotatoRaytracerEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) {
	// Create and clear uncertainty buffer
	uncertaintyBuffer = new Image<double>(windowWidth, windowHeight);
	uncertaintyBuffer->clear(0.0);
	
	Mat4d foxMat = rotateMat<double>(-DEG_90, DEG_90, 0) * translate(-1.0, -1.0, -1.0) * uniformScale(1.0/100.0);
	Mat4d vaseMat = translate(0.0, -1.0, 0.0) * uniformScale<double>(1.0/40);


	Mat4d modelMat = translate(0.0, -1.0, 0.0) * scale(1000.0, 0.1, 1000.0);
	PolyMeshd *floor = loadOBJTriangleMesh("sampleModels/cube.obj");
	floor->blendNormals = false;
	floor->uniformRecolor(Vec3d(0.8, 1.0, 0.9));
	floor->uniformRetexture(0.3, 0.4);
	floor->transform(modelMat);
	meshes.push_back(floor);


	modelMat = translate(0.0, 0.0, -3.0) * foxMat;
	PolyMeshd *fox = loadOBJTriangleMesh("sampleModels/fox.obj");
	fox->blendNormals = false;
	fox->uniformRecolor(Vec3d(1.0, 0.6, 0.1));
	fox->uniformRetexture(0.9, 0.1);
	fox->transform(modelMat);
	meshes.push_back(fox);

	modelMat = translate(0.0, 1.0, -7.0) * rotateMat(0.0, 1.2, 0.0) * scale(10.0, 10.0, 0.1);
	PolyMeshd *wall1 = loadOBJTriangleMesh("sampleModels/cube.obj");
	wall1->blendNormals = false;
	wall1->uniformRecolor(Vec3d(0.9, 1.0, 1.0));
	wall1->uniformRetexture(1.0, 0.0);
	wall1->transform(modelMat);
	meshes.push_back(wall1);

	modelMat = translate(0.0, 1.0, -7.0) * rotateMat(0.0, -1.2, 0.0) * scale(10.0, 10.0, 0.1);
	PolyMeshd *wall2 = loadOBJTriangleMesh("sampleModels/cube.obj");
	wall2->blendNormals = false;
	wall2->uniformRecolor(Vec3d(1.0, 1.0, 0.9));
	wall2->uniformRetexture(1.0, 0.2);
	wall2->transform(modelMat);
	meshes.push_back(wall2);
}

// Delete/clear all data
PotatoRaytracerEngine::~PotatoRaytracerEngine() {
    for(int i = 0; i < allMeshes.size(); i++) { 
        delete allMeshes.at(i); 
    } 
    allMeshes.clear(); 
    renderMeshes.clear();
	meshes.clear();
 
    allFragments.clear(); 

	uncertaintyBuffer->clear();
	delete uncertaintyBuffer;
}

// Draw everything to draw buffer; called once per drawing loop iteration
void PotatoRaytracerEngine::renderToDrawBuffer(Image<Vec3f> * drawBuffer) {
	buffer_passes++;
	// For each x,y
	for (int x = 0; x < windowWidth; x++) {
		for (int y = 0; y < windowHeight; y++) {
			// If using the uncertainty buffer to perform more raycasts in less certain areas
			if (USE_UNCERTAINTY) {
				// Get the uncertainty and round it to an int, then add 1
				int uncertainty = (int)(uncertaintyBuffer->getPixel(x,y)*UNCERTAINTY_SCALE)+1;
				double new_uncertainty = 0.0;
				Vec3d color = {};
				// Greater uncertainty ==> More raycasts
				for (int i = 0; i < uncertainty; i++) {
					double temp_uc; // Partial uncertainty passed by reference to next raycast
					Ray ray = Ray(windowWidth, windowHeight, x, y);
					color = color.mix(raycast(ray, temp_uc), 1.0/(double)(i+1)); // Blend the colors from each raycost into color
					new_uncertainty += temp_uc / (uncertainty); // Add partial uncertainty to the new total for the pixel
				}
				// For debugging the uncertainty buffer
				if (UNCERTAINTY_DEBUG)
					drawBuffer->setPixel(x,y, Vec3f((new_uncertainty/(new_uncertainty+UNCERTAINTY_SCALE))));
				else  {
					// Calcuate the weight that the new color will have based on the number of raycasts
					double weight = 1.0 - 1.0/(double)buffer_passes;
					for (int i = 1; i < uncertainty; i++) 
						weight *= weight;
					drawBuffer->setPixel(x,y,drawBuffer->getPixel(x,y).mix(color, 1.0-weight));
				}
				// Update the uncertainty buffer
				uncertaintyBuffer->setPixel(x,y, max(new_uncertainty, uncertaintyBuffer->getPixel(x,y)));
			}
			// If not using uncertainty, just do a single raycast and merge it with the current color in drawBuffer
			else {
				Ray ray = Ray(windowWidth, windowHeight, x, y);
				double temp;
				drawBuffer->setPixel(x, y, Vec3f(drawBuffer->getPixel(x,y)).mix(raycast(ray, temp), 1.0f/(float)buffer_passes));
			}
		}
	}
}

// Casts a ray from the camera and returns the final color for the pixel
Vec3f PotatoRaytracerEngine::raycast(Ray ray, double &uncertainty) {
	// Holds information about collisions
	Vertd col_vert;
	col_vert.roughness = 0.0; // Default roughness
	uncertainty = 0.0; // Default uncertainty

	// Bounce a ray a certain number of times, or until it stops hitting anything
	for (int i = 0; i < MAX_BOUNCES; i++) {
		double disatnce; // Pass into collideRay to get the distance traveled for the collision
		double uncertainty_factor = col_vert.roughness * ray.specular.length();
		if (collideRay(ray, col_vert, disatnce)) {
			// Get the diffuse scalar based on the diffuse color, normal direciton, and light direction
			double diffuse_scalar = (1.0 + MIN_SKY_LIGHT + col_vert.normal.dot(lightDirection)) / (2.0 + 2*MIN_SKY_LIGHT);
			
			// Reflect the ray
			if (ENABLE_RANDOM)
				ray.reflectSelf(col_vert, Vec3d(diffuse_scalar), Vec3d(randDouble(), randDouble(), randDouble()));
			else
				ray.reflectSelf(col_vert, Vec3d(diffuse_scalar));
			
			// Add uncertainty from this ray collision
			uncertainty += uncertainty_factor * disatnce;
		}
		else {
			// Add uncertainty for a ray hitting a surface and missing all objects
			uncertainty += uncertainty_factor * MISS_DISTANCE;
			break;
		}
	}
	
	// Scale the light based on direction of light "source"
	double spec_scalar = (1.0 + MIN_SKY_LIGHT + ray.direction.dot(lightDirection)) / (2.0 + 2*MIN_SKY_LIGHT);
	return Vec3f(ray.specular*spec_scalar + ray.diffuse);
}


struct BBCollideData {
	int meshIndex;
	double near_t;
	double far_t;
};

// Returns the color from the given raycast
bool PotatoRaytracerEngine::collideRay(Ray ray, Vertd &col_vert, double &distance) {
	
	// List of collisions with bounding boxes
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
			i,	// Mesh index
			t1,	// Closest collision (or ray.start if inside bounding box)
			t2	// Furthest collision
		};
		
		// If list of collides is empty, add first one
		if (!bb_collides.size()) {
			bb_collides.push_back(new_collide);
			continue;
		}

		// Insert into the list
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

	// Data about the closest collision (if any)
	Vertd closest_cld_vert;
	double closest_cld_t = -1;
	bool hasBeenACollide = false;

	// For each bounding box collision
	for (BBCollideData bb_cld : bb_collides) {
		// Get the corresponding mesh
		PolyMeshd *mesh = meshes.at(bb_cld.meshIndex);
		
		// Get the bounding box of the ray's intersection with the mesh's bounding box
		BoundBoxd bb = BoundBoxd(ray.posFromT(bb_cld.near_t), ray.posFromT(bb_cld.far_t));

		// If the closest possible collision is still further than another found collision
		if (hasBeenACollide && bb_cld.near_t > closest_cld_t)
			continue;
		
		// For each face
		for (int fi = 0; fi < mesh->getFaces().size(); fi++) {
			// Get the data for the face (each vertex, face normal, etc.)
			FaceData face = mesh->getFaceData(fi);
			
			// If the face doesn't inersect with the ray's bounding box
			if (face.getESCLCode(bb))
				continue; // Skip
			
			// Get the parameter of the collision with the face's plane
			double cld_t = ray.collide(face.v1.pos, face.normal);

			// If the ray must travel backwards to collide with face-plane
			if (cld_t < 0)
				continue; // Skip

			// If there was another closer collision
			if (hasBeenACollide && cld_t > closest_cld_t)
				continue; // Skip

			// If collision with plane is not within the ray's intersection bounding box
			if (cld_t > bb_cld.far_t || bb_cld.near_t > cld_t)
				continue; // Skip
			
			// Get barycentric coordinates of collision
			Vec3d pos = ray.posFromT(cld_t);
			Vec3d bary = bary3D(pos, face.v0.pos, face.v1.pos, face.v2.pos, face.normal);
			
			// If did not colide
			if (!isInside(bary))
				continue; // Skip to next face

			closest_cld_t = cld_t;
			closest_cld_vert = face.interpolateFaceBary(bary, false);
			closest_cld_vert.pos = pos;
			hasBeenACollide = true;
		}
	}

	// Put the new collision vert into the reference variable
	col_vert = closest_cld_vert;
	distance = closest_cld_t;
	// Return whether or not a collision was found
	return hasBeenACollide;
}