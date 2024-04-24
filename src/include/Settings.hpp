#pragma once 
 
// Comment to turn off testing 
#define ENABLE_TESTING 

#define BASICALLY_ZERO 0.0001
 
// Set target FPS 
#define TARGET_FPS      60 
#define USE_TARGET_FPS  false 
 
// Print out stats every X number of seconds 
#define AVE_STATS_TIME_WINDOW    1.0 
#define PRINT_AVE_STATS          true 
 
// Test geometry fan options 
#define GEO_FAN_BLADE_CNT           15 
 
// Drawing options 
#define DRAW_LINES_AS_WIREFRAME     true 
#define LINE_DDA                    0 
#define LINE_MID                    1 
//#define LINE_ALGORITHM              LINE_DDA 
#define LINE_ALGORITHM              LINE_MID 
 
// Renderer options 
#define BASE_RENDERER       0 
#define EXAMPLE_RENDERER    1 
#define FORWARD_RENDERER    2 
#define RAYTRACING_RENDERER	3
#define RENDERER_CHOICE     RAYTRACING_RENDERER 

// Clipping options
#define CLIP_LEFT -0.8f
#define CLIP_RIGHT 0.9f
#define CLIP_BOTTOM 0.1f
#define CLIP_TOP 0.75f
#define CLIP_NEAR -1.0f
#define CLIP_FAR 1.0f

// Raytracer options
#define MAX_BOUNCES 10
#define NEAR_PLANE 0.1
#define VIEW_HEIGHT NEAR_PLANE*1.3

#define RANDOMNESS 1.0
#define ENABLE_RANDOM true
#define AVERAGE_FRAMES true
#define USE_UNCERTAINTY true

#define VSYNC false
#define DOUBLE_BUFFERING false
#define CLEAR_DRAW_BUFFER_PER_FRAME false