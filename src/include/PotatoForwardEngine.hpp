#pragma once

#include "PotatoRenderEngine.hpp"
#include "Rasterize.hpp"
using namespace std;
using namespace potato;
using namespace tinyxml2;

class PotatoForwardEngine : public PotatoRenderEngine {
    private: 
        // List of triangle meshes
        vector<PolyMesh*> allMeshes;
        vector<Fragment> allFragments;   

        // Merge fragments
        void mergeFragments(vector<Fragment> &fragList, Image<Vec3f> *drawBuffer);
                
    public:
        PotatoForwardEngine(int windowWidth, int windowHeight);
        virtual ~PotatoForwardEngine();  
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer);
};
