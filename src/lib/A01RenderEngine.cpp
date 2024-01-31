#include "A01RenderEngine.hpp"

A01RenderEngine::A01RenderEngine(int windowWidth, int windowHeight) {
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

A01RenderEngine::~A01RenderEngine() { 
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

void A01RenderEngine::renderToWindowTexture() {    
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

void A01RenderEngine::clearBuffer(unsigned char *buffer, 
                                    unsigned char r,
                                    unsigned char g,
                                    unsigned char b) {
    for(int i = 0; i < totalBufferSize; i += nrComponents) {
        buffer[i] = r;
        buffer[i+1] = g;
        buffer[i+2] = b;        
    }
}

void A01RenderEngine::drawOneFrame() {
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

	// Draw rectangle
    int rectWidth = 200;
    int colInc = rectCol / 128 + 2;
    drawAABox(drawBuffer, rectCol, 0, (rectCol+rectWidth), windowHeight-1,
                0, 160, 255);
	// Draw "overflow" rectangle
	if (rectCol + rectWidth > windowWidth)
		drawAABox(drawBuffer, 0, 0, (rectCol+rectWidth) % windowWidth, windowHeight - 1, 0, 160, 255);

	// Draw elipse
	int rx = windowWidth / 10;
	int ry = (windowHeight / 6 + elipCol % 72);
	drawAnElipse(drawBuffer, elipCol + rx, (windowHeight / 2), 
				rx, ry, 255, 80, 0);
	// Draw "overflow" elipse
	if (elipCol + 2 * rx > windowWidth)
		drawAnElipse(drawBuffer, elipCol + rx - windowWidth, windowHeight / 2, rx, ry, 255, 80, 0);
	
	// Reset rectCol & elipCol
    rectCol = (rectCol+colInc);
	if (rectCol >= windowWidth) rectCol = 0;
	elipCol += colInc * 3;
	if (elipCol >= windowWidth) elipCol = 0;

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

void addToBuffer(unsigned char* buffer, int index,
			unsigned char r, unsigned char g, unsigned char b) {
	unsigned char colors[3] = {r, g, b};

	for (int i = 0; i < 3; i++) {
		if (buffer[index+i] == 0)
			buffer[index+i] = colors[i];
		else
			buffer[index+i] = (buffer[index+i] + colors[i]) / 2;
	}
}

void A01RenderEngine::drawAABox(  unsigned char* buffer,
                                    int sx, int sy, 
                                    int ex, int ey,
                                    unsigned char r,
                                    unsigned char g,
                                    unsigned char b) {

    int index = nrComponents*(windowWidth*sy + sx);
    int lineWidth = windowWidth*nrComponents;

	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;

    for(int y = sy; y <= ey && y < windowHeight; y++) {
        int startCol = index;
        for(int x = sx; x <= ex && x < windowWidth; x++) {
			addToBuffer(buffer, index, r, g, b);
            index += nrComponents;
        }
        index = startCol + lineWidth;
    } 
}

void A01RenderEngine::drawAnElipse(unsigned char* buffer,
									int cx, int cy, int rx, int ry,
									unsigned char r, unsigned char g, unsigned char b) {
	int sx = cx - rx;
	int sy = cy - ry;
	if (sx < 0) sx = 0;
	if (sy < 0) sy = 0;
	
	int ex = cx + rx;
	int ey = cy + ry;
	if (ex >= windowHeight) ex = windowWidth - 1;
	if (ey >= windowHeight) ey = windowHeight - 1;
	
	int index = nrComponents*(windowWidth*sy + sx);
    int lineWidth = windowWidth*nrComponents;

    for(int y = sy; y <= ey && y < windowHeight; y++) {
        int startCol = index;
        for(int x = sx; x <= ex && x < windowWidth; x++) {
			int relX = x - cx;
			int relY = y - cy;

			if ((double)(relX * relX) / (rx * rx) + (double)(relY * relY) / (ry * ry) <= 1) {
				addToBuffer(buffer, index, r, g, b);
			}
            index += nrComponents;
        }
        index = startCol + lineWidth;
    } 
}

