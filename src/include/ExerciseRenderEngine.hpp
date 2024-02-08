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
#include "Timing.hpp"
#include "Settings.hpp"
using namespace std;
using namespace potato;

class ExerciseRenderEngine {
    private:
        // Data for display buffer
        unsigned int windowTextureID = 0;        
        Image<Vec3u> *frontBuffer = 0;
        Image<Vec3u> *backBuffer = 0;
        Image<Vec3u> *screenBuffer = 0;

        int windowWidth = 0;
        int windowHeight = 0;

        // Threading stuff
        thread *drawThread = 0;
        bool drawThreadRunning = false;
        mutex frontBufferMutex;
        bool USE_VSYNC = true;

        // Timing stuff
        Timekeeper timekeeper;
        
        // BONUS    
        int currentCol = 0;

        // Internal drawing functions        
        void drawingLoop();    
        void drawAABox( Image<Vec3u> *buffer,
                        int sx, int sy, 
                        int ex, int ey,
                        Vec3u color);        
        void swapBuffers();
        
    public:
        ExerciseRenderEngine(int windowWidth, int windowHeight);
        ~ExerciseRenderEngine();  
        // Copies display buffer to window texture  
        void renderToWindowTexture();
};
