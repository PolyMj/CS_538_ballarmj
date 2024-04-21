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
#include "Line.hpp" 
#include "Mesh.hpp" 
#include "Buffer.hpp" 
#include "Timing.hpp" 
#include "Settings.hpp" 
#include "XML.hpp" 
#include "tinyxml2.h" 
using namespace std; 
using namespace potato; 
using namespace tinyxml2; 
 
class PotatoRenderEngine { 
    protected: 
        // Data for display buffer 
        unsigned int windowTextureID = 0; 
        Image<Vec3f> *frontBuffer = 0; 
        Image<Vec3f> *backBuffer = 0; 
        Image<Vec3u> *screenBuffer = 0; 
 
        int windowWidth = 0; 
        int windowHeight = 0; 
 
        // Threading stuff 
        thread *drawThread = 0; 
        bool drawThreadRunning = false; 
        mutex frontBufferMutex; 
        bool USE_VSYNC = false; 
 
        // Timing stuff 
        Timekeeper timekeeper; 
 
        // Internal drawing functions 
        void drawingLoop(); 
        void swapBuffers(); 
 
    public: 
        PotatoRenderEngine(int windowWidth, int windowHeight); 
        virtual ~PotatoRenderEngine(); 
        // Start thread running 
        void initialize(); 
        // Stop thread 
        void shutdown(); 
        // Copies display buffer to window texture 
        void renderToWindowTexture(); 
 
        // METHODS TO OVERRIDE 
        virtual void renderToDrawBuffer(Image<Vec3f> *drawBuffer); 
}; 
