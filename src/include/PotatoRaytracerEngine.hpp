#pragma once 
 
#include <random>
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

#define MIN_SKY_LIGHT 0.5
 
class PotatoRaytracerEngine : public PotatoRenderEngine { 
    private: 
        // List of triangle meshes 
        vector<PolyMesh*> allMeshes; 
        vector<Fragment> allFragments; 
        vector<PolyMesh*> renderMeshes {};

        Vec3d lightDirection = Vec3d(0.3, 1.0, 0.7).normalize();

        vector<PolyMeshd*> meshes;
        
        // Random number stuff
        mt19937 gen = mt19937(std::random_device{}());
        uniform_real<double> dis = uniform_real<double>(0, RANDOMNESS);
        inline double randDouble() { return dis(gen); };

        void PotatoRaytracerEngine::processAllGeomtry();
        void PotatoRaytracerEngine::processGeomtryOneMesh(Mat4f modelMat, Mat4f viewMat);

        Vec3f PotatoRaytracerEngine::raycast(Ray ray);
        bool PotatoRaytracerEngine::collideRay(Ray ray, Vertd &col_vert);
 
    public: 
        PotatoRaytracerEngine(int windowWidth, int windowHeight); 
        virtual ~PotatoRaytracerEngine(); 
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer); 
}; 
