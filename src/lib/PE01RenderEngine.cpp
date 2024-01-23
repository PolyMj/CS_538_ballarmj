#include "PE01RenderEngine.hpp"

PE01RenderEngine::PE01RenderEngine(int windowWidth, int windowHeight) {
    // Store window width and height
    // We assume RGB for the format (so 3 components)
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    this->nrComponents = 3;

    // Create drawing buffer and a "screen" buffer 
    // (as if we were transmitting information to the display device)
    this->totalBufferSize = windowWidth*windowHeight*nrComponents;
    this->frontBuffer = new unsigned char[totalBufferSize];     
    this->screenBuffer = new unsigned char[totalBufferSize];
    clearBuffer(this->frontBuffer, 0, 0, 0);    
    clearBuffer(this->screenBuffer, 0, 0, 0);

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
    drawThread = new thread(&PE01RenderEngine::drawOneFrame, this);
}

PE01RenderEngine::~PE01RenderEngine() { 
    // Clean up thread
    drawThreadRunning = false;
    drawThread->join();
    delete drawThread;
    drawThread = 0;

    // Clean up buffer(s)
    delete [] frontBuffer;  
    delete [] screenBuffer;  
    frontBuffer = 0;
    screenBuffer = 0;
    
    // Clean up texture    
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &(windowTextureID));
    windowTextureID = 0;
}

void PE01RenderEngine::renderToWindowTexture() {    
    // FOR NOW, just draw the frame here
    //drawOneFrame();

    // Activate screen texture    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, windowTextureID);

    // Simulate buffer to screen transfer    
    for(int i = 0; i < totalBufferSize; i++) {        
        screenBuffer[i] = frontBuffer[i];
    }
        
    // Copy in screen buffer to texture
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, windowWidth, windowHeight, 
                    GL_RGB, GL_UNSIGNED_BYTE, screenBuffer); 	
}

void PE01RenderEngine::clearBuffer(unsigned char *buffer, 
                                    unsigned char r,
                                    unsigned char g,
                                    unsigned char b) {
    for(int i = 0; i < totalBufferSize; i += nrComponents) {
        buffer[i] = r;
        buffer[i+1] = g;
        buffer[i+2] = b;        
    }
}

void PE01RenderEngine::drawOneFrame() {
    // Timing code from: https://www.learncpp.com/cpp-tutorial/timing-your-code/
    using Clock = std::chrono::steady_clock;
	using Second = std::chrono::duration<double, std::ratio<1> >;

    // Get appropriate total time for desired FPS
    // (Slightly off because of 15 ms in main program and copy times)
    double targetFPS = 60;
    double targetTime = 1.0/targetFPS;

    while(drawThreadRunning) {
        // Start time
        chrono::time_point<Clock> startTime = Clock::now();

        // Set drawing buffer
        unsigned char *drawBuffer = frontBuffer;

        // Clear drawing buffer
        clearBuffer(drawBuffer, 0, 0, 0);

        // Draw our items
        // EXAMPLE: Just draw a red column that moves every frame
        int colWidth = 200;
        int colInc = 1;
        drawAABox(drawBuffer, currentCol, 0, (currentCol+colWidth), windowHeight-1,
                    255, 0, 0);
        currentCol = (currentCol+colInc)%windowWidth;

        // Get elapsed time
        double elapsed = chrono::duration_cast<Second>(Clock::now() - startTime).count();

        // Calculate how much time to wait
        double waitTime = targetTime - elapsed;

        // Calculate possible FPS
        int possibleFPS = (int)(round(1.0/elapsed));

        // Overtime?
        if(waitTime < 0) {            
            waitTime = 0;
        }

        // Print time elapsed
        cout << "POSSIBLE FPS: " << possibleFPS << "; TIME (SECONDS): " << elapsed << endl;

        // Wait extra time
        this_thread::sleep_for(chrono::milliseconds((long)round(waitTime*1000)));    
    }
}

void PE01RenderEngine::drawAABox(  unsigned char* buffer,
                                    int sx, int sy, 
                                    int ex, int ey,
                                    unsigned char r,
                                    unsigned char g,
                                    unsigned char b) {

    int w = ex - sx + 1;
    int h = ey - sy + 1;
    int index = nrComponents*(windowWidth*sy + sx);
    int lineWidth = windowWidth*nrComponents;

    for(int y = sy; y <= ey && y < windowHeight; y++) {
        int startCol = index;
        for(int x = sx; x <= ex && x < windowWidth; x++) {
            buffer[index] = r;
            buffer[index+1] = b;
            buffer[index+2] = g;
            index += nrComponents;
        }
        index = startCol + lineWidth;
    } 
}
