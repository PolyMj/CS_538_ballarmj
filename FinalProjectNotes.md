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
[Sebastian Lague on YouTube - Coding Adventure: Ray Tracing](https://youtu.be/Qz0KTGYJtUk?si=mGgxsdjnIQHFab0f)
 - Walkthrough of making a basic raytracer in unity. 
 - Pretty clearly explains a lot of the necessary concepts for getting a raytracer made. 
 - I became irrationally angry at the number of rays you need for basic global illumination. 