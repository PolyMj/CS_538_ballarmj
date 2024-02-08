#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
#include "Vector.hpp"
#include "Buffer.hpp"
using namespace std;
using namespace potato;

class BasicRenderEngine {
    private:
        // Data for display buffer
        unsigned int windowTextureID = 0;
        // unsigned char *frontBuffer = 0;
        // unsigned char *screenBuffer = 0;
		Image<Vec3u> * frontBuffer = 0;
		Image<Vec3u> * screenBuffer = 0;


        int windowWidth = 0;
        int windowHeight = 0;
        int nrComponents = 3;
        int totalBufferSize = 0;

        // BONUS    
        int currentCol = 0;

        // Internal drawing functions
        void clearBuffer(   Image<Vec3u> *buffer, 
                            Vec3u);
        void drawOneFrame();    
        void drawAABox( Image<Vec3u> *buffer,
                        int sx, int sy, 
                        int ex, int ey,
                        Vec3u color);
        
    public:
        BasicRenderEngine(int windowWidth, int windowHeight);
        ~BasicRenderEngine();  
        // Copies display buffer to window texture  
        void renderToWindowTexture();
};
