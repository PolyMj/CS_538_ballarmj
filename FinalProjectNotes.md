# 
# Plans
 - Before 4/20
   - Create a raytracer that can shoot rays at a scene with a single polygon and show the shape on the screen. 
   - Expand to allow for checking multiple polygons. 
   - Expand to check on a model-by-model basis, using bounding boxes to optimize these checks. 
   - Focus will be in direct/specular lighting, but diffuse lighting will be simulated if time allows. 
 - ~~After 4/20~~
   - ~~I'll try using similations to define a function that can be used to approximate the ambient lighting behaviour of a model. ~~
   - ~~These "ambient functions" will take in all light sources during an initial lighting pass, then during rendering will take in a point and return the influence they have on the lighting of that point. ~~
   - ~~All of this will likely be on a separate branch, called something like "final_project_ambiance"~~
   - Scratch literally all of this, I just get excited at dumb ideas and jump to step 10 before I've started step 1

#
# Misc Notes
### 4/18/2024
 - Need to begin working on relfecting rays, and I'm deciding how I should store/modify the final color
   - ray.color starts at (1.0f, 1.0f, 1.0f), then as it hits objects gets reduced by however much light is absorbed. 
   - When it hits a light source, the remaining color will be multiplied by the light color, then returned as the final color. 
   - Can stop if the remaing color gets too low? Or maybe that's just asking for problems. 
 - Ray-collision optimizations
   - After one collision check, my program has slowed to 6fps, so I will absolutely need to do at least some basic optimizations. 
   - I remember hearing Reale mention using bounding boxes for this. Maybe around each model and polygon?
   - There's also some include files in this project that deal with bounding boxes, so I should take at look at them for inspiration. 
   - I could use Chris Taile's plane-intersection to find the intersection point on a plane defined using the model's center and the ray's direction as the normal
     - It will always intersect, but then you can find the distance from the intersection to the center of the model pretty easily. 
 - Importing models
   - The code for importing models does already exists, but there are no normals, colors, textures, etc., which will need to be implemented in some way.
 - Overall structure of Code
   - I'm having trouble understanding how this project is structured overall. My raytracer's drawing is called using renderToDrawBuffer(), but I can't trace it back to any calls starting from PotatoRenderApp.cpp. I tried using VSCode's search function to search all files but can't find any references to this function other than in PotatoRenderEngine.cpp, but it's in a function that I also can't trace back. 
   - I need to figure out the structure so I can know where to load models, move the camera, and other application-level operations. Currently, I don't know how to communicate any of this data to my raytracers. 
   - "117? That's an number of commitment." - Michael J. Reale
 - Important Notes
   - Change renderer to not do double buffering and to draw while rendering
   - Turn off VSync
   - Setting to disable buffer clearing
   - Change from floats to doubles
   - Threadpool stuff?
     - Just doing one row at a time? 
   - 
### 4/22/2024
  - Store last face and collision point, then only interpolate with barycentric after you know for certain that the given collision is the closest one.

#
# Resources
### [Sebastian Lague on YouTube - Coding Adventure: Ray Tracing](https://youtu.be/Qz0KTGYJtUk?si=mGgxsdjnIQHFab0f)
 - Walkthrough of making a basic raytracer in unity. 
 - Pretty clearly explains a lot of the necessary concepts for getting a raytracer made. 
 - I became irrationally angry at the number of rays you need for basic global illumination. 

### [Contemporary Calculus - 11.7: Vector Reflections](https://www.contemporarycalculus.com/dh/Calculus_all/CC11_7_VectorReflections.pdf)
 - Gives the formula for finding vector reflections

### [Chris Tralie on YouTube - Ray Intersect Plane](https://youtu.be/x_SEyKtCBPU?si=JpSmm9JIxuqC_I3g)
 - ray_start = p0, ray_direction = v, parameter = t, point_on_plane = a, normal = n
   - ((p0-a) + tv) . n = 0
   - (p0-a).n + tv.n = 0
   - tv.n = -(p0-a).n
   - t = -(p0-a).n / v.n
   - t = (a-p0).n / v.n
   - If (v.n == 0) ==> Parallel
   - If (t < 0) ==> Opposite of the ray intersects, not the actual ray

### [Chris Trailie on YouTube - Ray Intersect Triangle](https://youtu.be/zAhBA42n1eg?si=_6uAikKcf_m9IKqD)
 - Use intersection with plane to get t ==> (p) point of intersection between ray and plane
 - Use barycentric(p) to check if p is in the triangle