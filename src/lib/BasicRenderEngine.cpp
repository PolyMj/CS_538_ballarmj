#include "BasicRenderEngine.hpp"

BasicRenderEngine::BasicRenderEngine(int windowWidth, int windowHeight) {
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
}

BasicRenderEngine::~BasicRenderEngine() { 
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

void BasicRenderEngine::renderToWindowTexture() {    
    // FOR NOW, just draw the frame here
    drawOneFrame();

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

void BasicRenderEngine::clearBuffer(unsigned char *buffer, 
                                    unsigned char r,
                                    unsigned char g,
                                    unsigned char b) {
    for(int i = 0; i < totalBufferSize; i += nrComponents) {
        buffer[i] = r;
        buffer[i+1] = g;
        buffer[i+2] = b;        
    }
}

void BasicRenderEngine::drawOneFrame() {
    // Timing code from: https://www.learncpp.com/cpp-tutorial/timing-your-code/
    using Clock = std::chrono::steady_clock;
	using Second = std::chrono::duration<double, std::ratio<1> >;

    // Get appropriate total time for desired FPS
    // (Slightly off because of 15 ms in main program and copy times)
    double targetFPS = 60;
    double targetTime = 1.0/targetFPS;

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

    potato::Vec3f A(1.0f,2.0f,3.0f);
    potato::Vec3f B(4.5f, 6.7f, 8.9f);
    cout << "LENGTH: " << A.length() << endl;
    potato::Vec3f C = A + B;
    C[1] = 5280.1f;
    cout << "C: " << C << endl;

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

void BasicRenderEngine::drawAABox(  unsigned char* buffer,
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
            buffer[index+1] = g;
            buffer[index+2] = b;
            index += nrComponents;
        }
        index = startCol + lineWidth;
    } 
}
