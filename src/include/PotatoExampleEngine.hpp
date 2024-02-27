#pragma once

#include "PotatoRenderEngine.hpp"
using namespace std;
using namespace potato;
using namespace tinyxml2;

class PotatoExampleEngine : public PotatoRenderEngine {
    private:   
        // Current column position
        int currentCol = 0;     
        
        // Internal drawing functions     
        void drawAABox( Image<Vec3f> *buffer,
                        int sx, int sy, 
                        int ex, int ey,
                        Vec3f color);        
                
    public:
        PotatoExampleEngine(int windowWidth, int windowHeight);
        virtual ~PotatoExampleEngine();  
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer);
};
