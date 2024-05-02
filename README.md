# Overview
### **Project Name:** Basic Raytracing Engine  
### **Author (of raytracing portion):** Michael Ballard : Student at SUNY Polytechnic Institute  
### **Class:** CS-538 : Algorithms in Computer Graphics with Dr. Michael J. Reale
### **Description:**
A basic raytracing engine coded in C++ which loads triangle meshes from .obj files and renders them by casting rays from the origin/camera. Vertices and faces are loaded from .obj files, normals are computed from this data, and material information (specular color, diffuse color, roughness, and metallicity) is set manually. Unlike most raytracers, this engine uses a "light direction", and assumes that if a ray doesn't collide with anything, it's hitting some sort of light source, with the magnitude of this light being propotional to the allignment of the ray's direction and the light direciton. 

# Running the Project
### Debug Target: PotatoRenderApp
### Arguments: None
### Placing Custom Models:
In "src/lib/PotatoRaytracerEngine.cpp", models are loaded in the constructor in the area bounded with comments saying "LOAD MODELS" and "DONE LOADING MODELS". Use the following format to load, recolor, retexture, and transform a model:  
 - **Note:** [*ITALICIZED_CAPS_SQBRACKETS*] means it's a value you need to fill in. MESH and MAT can be any unused variable name. 
 - Mat4d [*MAT*] = [*MATRIX_TRANSFORMATIONS*];
 - PolyMeshd \*[*MESH*] = loadOBJTriangleMesh("[*MESH_FILE_LOCATION*]");
   - File loc is relative to this project, 
 - [*MESH*]->blendNormals = [*true/false*];
 - [*MESH*]->uniformRecolor( [*DIFFUSE_COLOR*], [*SPECULAR_COLOR*] );
   - Colors are formating as: Vec3d(R, G, B) : with R,G,B being values 0.0 ~ 1.0
 - [*MESH*]->uniformRetexture( [*METALLICITY*], [*ROUGHNESS*] );
   - With metallicity and roughness being values 0.0 ~ 1.0
 - [*MESH*]->transform( [*MAT*] );
   - with MAT being a 4x4 matrix transformation
   - Available functions: translate(x,y,z), rotateMat(yaw, pitch, roll), scale(x,y,z)
     - Ensure that each value is a double, e.g. write "1.0" instead of "1" or "1.0f"
   - See "src/include/Matrix.hpp" for actual code on creating matrices
 - meshes.push_back( [*MESH*] );  

If you want to disable a model that's already there, comment out the line conatining "meshes.push_back...", or delete all code relating to that model only. 

### Settings (src/include/SETTINGS.hpp):
 - MAX_BOUNCES : The maximum number of ray reflections per pixel
 - NEAR_PLANE : The cutoff point where objects closer than this distance will no longer render
 - VIEW_HEIGHT : The height of the virtual screen. Essentially FOV but less convenient to adjust. 
 - ENABLE_RANDOM : Enable random ray scattering (disabling will essentially disable rough surfaces)
 - USE_UNCERTAINTY : Use the uncertainty buffer, which keeps track of the uncertainty of the color of each pixel, and performs more calculations on less-certain pixels to improve accuracy and efficiency. Causes later render passed to take longer. 
 - UNCERTAINTY_SCALE : A scalar applied to all values added to the uncertainty buffer. Increase this if you want to repeat more ray casts on rougher / more random surfaces.
 - MISS_DISTANCE : A bit hard to explain, but increasing will increase the measured uncertainty for a ray that doesn't collide with anything (misses).
 - PASSES_BEFORE_UNCERTAINTY : How many normal rendering passes will happen before it will use the uncertainty buffer. Increase this if there are too many areas remaining grainy after the first uncertainty render pass. 
 - UNCERTAINTY_DEBUG : Displays the uncertainty buffer to the screen instead of the actual color of the objects. Rougher objects should look white, perfect mirros that reflect light to nothing but other mirrors should be pure black. 

### Sample screenshots
![Render using a slightly yellow brushed metal wall on the left, a near-perfect mirror on the right, a matte-white floor, and an orange metallic fox](/screenshots/FinalProject/v3%20First%20Uncertainty%20Pass%20(16x%20as%20long%20to%20render).png "Render of the objects currently being loaded by the engine")

![Render with USE_RANDOMNESS set to false, leading to perfect, mirror-like reflections from all objects](/screenshots/FinalProject/No%20randomness.png "Using USE_RANDOMNESS false to remove randomness, essentially making all surfaces smooth and mirror-like")

![Grainy, speckeled, black and white rendering of the above image](/screenshots/FinalProject/v3%20Uncertainty%20Buffer%20Third%20Pass.png "Using UNCERTAINTY_DEBUG true to render the uncertainty buffer to the screen")

# Core Files
### New Files:
 - Meshd.hpp / Meshd.[hpp/cpp]:
   - Near-copies of Mesh.[hpp/cpp], but use doubles instead of floats. Meshd.hpp containes member functions for PolyMeshd to recolor, retecture, and transform models, and to recompute normals for faces and vertices. 
   - Added a FaceDada struct which contains the 3 vertices and normal of the face. PolyMeshd.getFaceData(int face_index) returns a FaceData struct for the corresponding face. 
   - Copied the loadOBJModel function from OBJModel.cpp and modified it to use structs from Meshd.hpp rather than Mesh.hpp.
 - PotatoRayTracerEngine.[hpp/cpp]:
   - Contains the bulk of the raytracing code
   - Loads models, casts rays from each pixel, and renders the scene to the drawBuffer, which is rendered to the scene. 
 - Ray.hpp
   - Defines the Ray struct, which contains information about individual rays, and has member functions to reflect itself off of surfaces and check intersections with bounding boxes. 
 - **Models:**
   - fox.obj : A low-poly fox
   - Vase.obj : A low-poly vase
   - Leanto.obj and "Leanto Roof.obj" : A small shack that I modeled from some reference images of one my family owns. 
     - Waring: High polygon count
   - "New Hexaeder.obj" : My remodeling of a 3D printable case for SlimeVR full-body trackers (see references for original by MaddesJG)
     - Waring: High polygon count

### Modified files
 - BounBox.hpp, Clipping.hpp, Bary.hpp
   - Various small modifications (mostly function templating or overloads) to allow for use of doubles instead of floats.
 - Bary.hpp
   - Added a function to interpolat between 3 values using BaryData.
 - Matrix.hpp
   - Added functions for creation of translation, scaling, and rotation matrices. 


# Dependencies
 - CMake
 - GLFW
 - GLEW
 - GLM
 - Assimp
 - Vulkan SDK
 - Doctest

# References
### [Sebastian Lague (on YouTube) - Coding Adventure: Ray Tracing](https://youtu.be/Qz0KTGYJtUk?si=mGgxsdjnIQHFab0f)
 - Walkthrough of making a basic raytracer in unity. 
 - Pretty clearly explains a lot of the necessary concepts for getting a raytracer made. 
 - I became irrationally angry at the number of rays you need for basic global illumination. 

### [Dale Hoffman - Contemporary Calculus : 11.7 : Vector Reflections](https://www.contemporarycalculus.com/dh/Calculus_all/CC11_7_VectorReflections.pdf)
 - Gives the formula for finding vector reflections. 

### [Chris Tralie (on YouTube) - Ray Intersect Plane](https://youtu.be/x_SEyKtCBPU?si=JpSmm9JIxuqC_I3g)
 - Given {Ray.start = s, Ray.direction = v, point_on_plane = p, face_normal = n, distance_to_collision = t}, solving for t:
   - ((s-p) + tv) . n = 0
   - (s-p).n + tv.n = 0
   - tv.n = -(s-p).n 
   - t = (p-s).n / v.n
   - (v.n == 0) ==> Parallel ==> No collision
   - (t < 0) ==> Ray must travel backwards ==> No collision


### [Chris Trailie (on YouTube) - Ray Intersect Triangle](https://youtu.be/zAhBA42n1eg?si=_6uAikKcf_m9IKqD)
 - Use intersection with plane to get t ==> (p) point of intersection between ray and plane
 - Use barycentric(p) to check if p is in the triangle


### [MaddesJG (on Thingiverse) - SlimeVR Case \"Hexaeder\"](https://www.thingiverse.com/thing:5140456)
 - Nearly perfect model for SlimeVR trackers, although the design of the rear-port for the female JST 5-pin connector means that disassembly requires descruction of the case or desoldering of components, which is why I redesigned it slightly. 
 - 