# 
# Plans
 - Before 4/20
   - Create a raytracer that can shoot rays at a scene with a single polygon and show the shape on the screen. 
   - Expand to allow for checking multiple polygons. 
   - Expand to check on a model-by-model basis, using bounding boxes to optimize these checks. 
   - Focus will be in direct/specular lighting, but diffuse lighting will be simulated if time allows. 
 - After 4/20
   - I'll try using similations to define a function that can be used to approximate the ambient lighting behaviour of a model. 
   - These "ambient functions" will take in all light sources during an initial lighting pass, then during rendering will take in a point and return the influence they have on the lighting of that point. 
   - All of this will likely be on a separate branch, called something like "final_project_ambiance"



#
# Resources
### [Sebastian Lague on YouTube - Coding Adventure: Ray Tracing](https://youtu.be/Qz0KTGYJtUk?si=mGgxsdjnIQHFab0f)
 - Walkthrough of making a basic raytracer in unity. 
 - Pretty clearly explains a lot of the necessary concepts for getting a raytracer made. 
 - I became irrationally angry at the number of rays you need for basic global illumination. 

### [Contemporary Calculus 11.7 Vector Reflections](https://www.contemporarycalculus.com/dh/Calculus_all/CC11_7_VectorReflections.pdf)
 - Gives the formula for finding vector reflections

### [Chris Tralie on YouTube - Ray Intersect Plane]
 - ray-start = p0, ray-direction = v, parameter = t, point-on-plane = a, normal = n
   - ((p0-a) + tv) . n = 0
   - (p0-a).n + tv.n = 0
   - tv.n = -(p0-a).n
   - t = -(p0-a).n / v.n
   - t = (a-p0).n / v.n
   - If (v.n == 0) ==> Parallel
   - If (t < 0) ==> Opposite of the ray intersects, not the actual ray

### [Chris Trailie on YouTube - Ray Intersect Triangle]
 - Use intersection with plane to get t ==> (p) point of intersection between ray and plane
 - Use barycentric(p) to check if p is in the triangle