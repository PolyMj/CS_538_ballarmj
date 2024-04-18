#pragma once 
 
#include "PotatoRenderEngine.hpp" 
#include "Rasterize.hpp" 
#include "OBJModel.hpp"
#include "Matrix.hpp"
// #include "Clipping.hpp"
#include "Ray.hpp"
using namespace std; 
using namespace potato; 
using namespace tinyxml2; 

// Temporary, just for getting things started
struct Triangle {
	Vert A;
	Vert B;
	Vert C;
	Vec3f normal;

	void computeNormal() {
		Vec3f AB = B.pos - A.pos;
		Vec3f AC = C.pos - A.pos;
		normal = AB.cross(AC).normalize();
	}
};
 
class PotatoRaytracerEngine : public PotatoRenderEngine { 
    private: 
        // List of triangle meshes 
        vector<PolyMesh*> allMeshes; 
        vector<Fragment> allFragments; 
        vector<PolyMesh*> renderMeshes {};

        Vec4f cameraPos = Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
        Vec4f cameraLookat = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

        Triangle tri;

        void PotatoRaytracerEngine::processAllGeomtry();
        void PotatoRaytracerEngine::processGeomtryOneMesh(Mat4f modelMat, Mat4f viewMat);

        Vec3f PotatoRaytracerEngine::raycast(Ray ray);
 
    public: 
        PotatoRaytracerEngine(int windowWidth, int windowHeight); 
        virtual ~PotatoRaytracerEngine(); 
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer); 
}; 
