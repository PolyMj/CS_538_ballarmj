#ifndef RENDER_ENGINE_H
#define RENDER_ENGINE_H

#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <mutex>
#include <cmath>
#include <GL/glew.h>					
#include <GLFW/glfw3.h>
using namespace std;

class E01RenderEngine {
    private:
        // Data for display buffer
        unsigned int windowTextureID = 0;
        unsigned char *frontBuffer = 0;
        unsigned char *screenBuffer = 0;
        int windowWidth = 0;
        int windowHeight = 0;
        int nrComponents = 3;
        int totalBufferSize = 0;

		thread *drawThread = 0;
		bool drawThreadRunning = false;

        // BONUS    
        int currentCol = 0;

        // Internal drawing functions
        void clearBuffer(   unsigned char *buffer, 
                            unsigned char r,
                            unsigned char g,
                            unsigned char b);
        void drawOneFrame();    
        void drawAABox( unsigned char *buffer,
                        int sx, int sy, 
                        int ex, int ey,
                        unsigned char r,
                        unsigned char g,
                        unsigned char b);
        
    public:
        E01RenderEngine(int windowWidth, int windowHeight);
        ~E01RenderEngine();  
        // Copies display buffer to window texture  
        void renderToWindowTexture();
};

#endif
