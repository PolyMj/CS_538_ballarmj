#include "PotatoExampleEngine.hpp" 
 
PotatoExampleEngine::PotatoExampleEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) { 
    // Nothing specific to do here 
} 
 
PotatoExampleEngine::~PotatoExampleEngine() { 
    // Also nothing specific 
} 
 
void PotatoExampleEngine::drawAABox(Image<Vec3f>* buffer, 
                                    int sx, int sy, 
                                    int ex, int ey, 
                                    Vec3f color) { 
 
    int w = ex - sx + 1; 
    int h = ey - sy + 1; 
 
    for(int y = sy; y <= ey && y < windowHeight; y++) { 
        for(int x = sx; x <= ex && x < windowWidth; x++) { 
            buffer->setPixel(x,y,color); 
        } 
    } 
} 
 
void PotatoExampleEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) { 
    int colWidth = 200; 
    int colInc = 1; 
    drawAABox(drawBuffer, currentCol, 0, (currentCol+colWidth), windowHeight-1, Vec3f(1.0, 0, 0)); 
    currentCol = (currentCol+colInc)%windowWidth; 
} 
