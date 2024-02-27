#include "PotatoForwardEngine.hpp"

PotatoForwardEngine::PotatoForwardEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) {
    // For now, generate simple fan
    PolyMesh *m = generateTestFan(Vec3f(windowWidth/2.0f, windowHeight/2.0f, 0.0f),
                                    windowHeight/3.0f, GEO_FAN_BLADE_CNT);
    allMeshes.push_back(m);
}

PotatoForwardEngine::~PotatoForwardEngine() { 
    // Clean up meshes
    for(int i = 0; i < allMeshes.size(); i++) {
        delete allMeshes.at(i);
    }
    allMeshes.clear();

    allFragments.clear();
}

void PotatoForwardEngine::mergeFragments(vector<Fragment> &fragList, Image<Vec3f> *drawBuffer) {
    // For now, just blindly write all fragments to buffer
    for(int i = 0; i < fragList.size(); i++) {
        Fragment f = fragList.at(i);
        drawBuffer->setPixel(f.pos.x, f.pos.y, Vec3f(f.color));
    }
}

void PotatoForwardEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) {
    // TODO

    // Clear list of fragments
    allFragments.clear();

    for(int i = 0; i < allMeshes.size(); i++) {
        PolyMesh *mesh = allMeshes.at(i);

        // Get fragments for inside of polygons
        fillTriangles(mesh, allFragments);

        // Get fragments for lines
        drawLines(mesh, allFragments, DRAW_LINES_AS_WIREFRAME);
    }

    // Merge fragments
    mergeFragments(allFragments, drawBuffer);
}
   
