#pragma once 
 
#include "PotatoRenderEngine.hpp" 
#include "Rasterize.hpp" 
#include "OBJModel.hpp"
#include "Matrix.hpp"
#include "Clipping.hpp"
using namespace std; 
using namespace potato; 
using namespace tinyxml2; 
 
class PotatoForwardEngine : public PotatoRenderEngine { 
    private: 
        // List of triangle meshes 
        vector<PolyMesh*> allMeshes; 
        vector<Fragment> allFragments; 
        vector<PolyMesh*> renderMeshes {};
 
        // Merge fragments 
        void mergeFragments(vector<Fragment> &fragList, Image<Vec3f> *drawBuffer);

        void processGeometryOneMesh(PolyMesh *inputMesh, Mat4f &modelMat, Mat4f &viewMat, Mat4f &projMat, PolyMesh *outMesh); 
 
    public: 
        PotatoForwardEngine(int windowWidth, int windowHeight); 
        virtual ~PotatoForwardEngine(); 
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer); 
}; 
