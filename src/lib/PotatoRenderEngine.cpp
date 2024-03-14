#include "PotatoRenderEngine.hpp" 
 
PotatoRenderEngine::PotatoRenderEngine(int windowWidth, int windowHeight) { 
    // Store window width and height 
    this->windowWidth = windowWidth; 
    this->windowHeight = windowHeight; 
 
    // Create drawing buffer and a "screen" buffer 
    // (as if we were transmitting information to the display device) 
    this->frontBuffer = new Image<Vec3f>(windowWidth, windowHeight); 
    this->backBuffer = new Image<Vec3f>(windowWidth, windowHeight); 
    this->screenBuffer = new Image<Vec3u>(windowWidth, windowHeight); 
    this->frontBuffer->clear(Vec3f(0,0,0)); 
    this->screenBuffer->clear(Vec3u(0,0,0)); 
 
    // Generate window texture 
    glGenTextures(1, &windowTextureID); 
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 
 
    glBindTexture(GL_TEXTURE_2D, windowTextureID); 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer->data()); 
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); 
 
    glBindTexture(GL_TEXTURE_2D, 0); 
} 
 
PotatoRenderEngine::~PotatoRenderEngine() { 
 
    // Clean up buffer(s) 
    delete frontBuffer; 
    delete backBuffer; 
    delete screenBuffer; 
    frontBuffer = 0; 
    backBuffer = 0; 
    screenBuffer = 0; 
 
    // Clean up texture 
    glBindTexture(GL_TEXTURE_2D, 0); 
    glDeleteTextures(1, &(windowTextureID)); 
    windowTextureID = 0; 
} 
 
void PotatoRenderEngine::initialize() { 
    // Create and start thread 
    drawThreadRunning = true; 
    drawThread = new thread(&PotatoRenderEngine::drawingLoop, this); 
} 
 
void PotatoRenderEngine::shutdown() { 
    // Clean up thread 
    drawThreadRunning = false; 
    if(drawThread != nullptr) { 
        drawThread->join(); 
        delete drawThread; 
        drawThread = 0; 
    } 
} 
 
void PotatoRenderEngine::renderToWindowTexture() { 
    // Activate screen texture 
    glActiveTexture(GL_TEXTURE0); 
    glBindTexture(GL_TEXTURE_2D, windowTextureID); 
 
    // Simulate buffer to screen transfer 
    if(USE_VSYNC) frontBufferMutex.lock(); 
    screenBuffer->copyFrom(frontBuffer, 255.0f, 0.0f); 
    if(USE_VSYNC) frontBufferMutex.unlock(); 
 
    // Copy in screen buffer to texture 
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, windowWidth, windowHeight, 
                    GL_RGB, GL_UNSIGNED_BYTE, screenBuffer->data()); 
} 
 
void PotatoRenderEngine::drawingLoop() { 
 
    while(drawThreadRunning) { 
        // Start time 
        timekeeper.startFrame(); 
 
        // Set drawing buffer 
        Image<Vec3f> *drawBuffer = backBuffer; 
 
        // Clear drawing buffer 
        drawBuffer->clear(); //(Vec3f(0,0,0)); 
 
        // Draw our items 
        renderToDrawBuffer(drawBuffer); 
 
        // Swap buffers 
        swapBuffers(); 
 
        // Get wait time 
        double waitTime = timekeeper.endFrame(); 
 
        // Wait extra time 
        if(USE_TARGET_FPS) { 
            this_thread::sleep_for(chrono::microseconds((long)round(waitTime*1000000.0))); 
        } 
        else { 
            this_thread::sleep_for(chrono::milliseconds(1)); 
        } 
    } 
} 
 
void PotatoRenderEngine::swapBuffers() { 
    if(!USE_VSYNC || frontBufferMutex.try_lock()) { 
        Image<Vec3f> *tmp = backBuffer; 
        backBuffer = frontBuffer; 
        frontBuffer = tmp; 
        if(USE_VSYNC) frontBufferMutex.unlock(); 
    } 
} 
 
void PotatoRenderEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) { 
    // DO NOTHING 
} 
