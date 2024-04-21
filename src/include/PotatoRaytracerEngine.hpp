#pragma once 
 
#include "PotatoRenderEngine.hpp" 
#include "Rasterize.hpp" 
#include "Meshd.hpp"
#include "Matrix.hpp"
#include "Ray.hpp"
// Note: Ensure that you use the correct structs/classes.
// Most of the files in this project were made for other programs and use floats instead of doubles. 
using namespace std; 
using namespace potato; 
using namespace tinyxml2; 
 
class PotatoRaytracerEngine : public PotatoRenderEngine { 
    private: 
        // List of triangle meshes 
        vector<PolyMesh*> allMeshes; 
        vector<Fragment> allFragments; 
        vector<PolyMesh*> renderMeshes {};

        Vec4f cameraPos = Vec4f(0.0f, 0.0f, 1.0f, 1.0f);
        Vec4f cameraLookat = Vec4f(0.0f, 0.0f, 0.0f, 1.0f);

        vector<PolyMeshd*> meshes;

        void PotatoRaytracerEngine::processAllGeomtry();
        void PotatoRaytracerEngine::processGeomtryOneMesh(Mat4f modelMat, Mat4f viewMat);

        Vec3f PotatoRaytracerEngine::raycast(Ray ray);
 
    public: 
        PotatoRaytracerEngine(int windowWidth, int windowHeight); 
        virtual ~PotatoRaytracerEngine(); 
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer); 
}; 
