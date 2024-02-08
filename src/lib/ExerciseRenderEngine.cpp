#include "ExerciseRenderEngine.hpp"

ExerciseRenderEngine::ExerciseRenderEngine(int windowWidth, int windowHeight) {
    // Store window width and height    
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    
    // Create drawing buffer and a "screen" buffer 
    // (as if we were transmitting information to the display device)    
    this->frontBuffer = new Image<Vec3u>(windowWidth, windowHeight);  
    this->backBuffer = new Image<Vec3u>(windowWidth, windowHeight);  
    this->screenBuffer = new Image<Vec3u>(windowWidth, windowHeight);
    this->frontBuffer->clear(Vec3u(0,0,0));
    this->screenBuffer->clear(Vec3u(0,0,0));

    // Generate window texture
    glGenTextures(1, &windowTextureID);    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    
    glBindTexture(GL_TEXTURE_2D, windowTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    // Create thread
    drawThreadRunning = true;
    drawThread = new thread(&ExerciseRenderEngine::drawingLoop, this);
}

ExerciseRenderEngine::~ExerciseRenderEngine() { 
    // Clean up thread
    drawThreadRunning = false;
    drawThread->join();
    delete drawThread;
    drawThread = 0;

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

void ExerciseRenderEngine::renderToWindowTexture() {    
    // FOR NOW, just draw the frame here
    //drawOneFrame();

    // Activate screen texture    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, windowTextureID);

    // Simulate buffer to screen transfer     
    if(USE_VSYNC) frontBufferMutex.lock();
    screenBuffer->copyFrom(frontBuffer);
    if(USE_VSYNC) frontBufferMutex.unlock();
        
    // Copy in screen buffer to texture
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, windowWidth, windowHeight, 
                    GL_RGB, GL_UNSIGNED_BYTE, screenBuffer->data()); 	
}

void ExerciseRenderEngine::drawingLoop() {   
    
    while(drawThreadRunning) {
        // Start time
        timekeeper.startFrame();

        // Set drawing buffer
        Image<Vec3u> *drawBuffer = backBuffer;

        // Clear drawing buffer
        drawBuffer->clear(Vec3u(0,0,0));

        // Draw our items
        // EXAMPLE: Just draw a red column that moves every frame
        int colWidth = 200;
        int colInc = 1;
        drawAABox(drawBuffer, currentCol, 0, (currentCol+colWidth), windowHeight-1,
                Vec3u(255, 0, 0));
        currentCol = (currentCol+colInc)%windowWidth;

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

void ExerciseRenderEngine::swapBuffers() {
    if(!USE_VSYNC || frontBufferMutex.try_lock()) {
        Image<Vec3u> *tmp = backBuffer;
        backBuffer = frontBuffer;
        frontBuffer = tmp;
        if(USE_VSYNC) frontBufferMutex.unlock();
    }
}

void ExerciseRenderEngine::drawAABox(   Image<Vec3u>* buffer,
                                        int sx, int sy, 
                                        int ex, int ey,
                                        Vec3u color) {

    int w = ex - sx + 1;
    int h = ey - sy + 1;
    
    for(int y = sy; y <= ey && y < windowHeight; y++) {
        for(int x = sx; x <= ex && x < windowWidth; x++) {            
            buffer->setPixel(x,y,color);            
        }        
    } 
}
