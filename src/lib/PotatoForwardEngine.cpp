#include "PotatoForwardEngine.hpp" 

PotatoForwardEngine::PotatoForwardEngine(int windowWidth, int windowHeight) : PotatoRenderEngine(windowWidth, windowHeight) { 
    // For now, generate simple fan 
    // PolyMesh *m = generateTestFan(Vec3f(windowWidth/2.0f, windowHeight/2.0f, 0.0f), 
    //                                 windowHeight/3.0f, GEO_FAN_BLADE_CNT); 
    PolyMesh *m = loadOBJModel("./sampleModels/teapot.obj");

    allMeshes.push_back(m); 
    renderMeshes.push_back(new PolyMesh(m));
} 
 
PotatoForwardEngine::~PotatoForwardEngine() { 
    // Clean up meshes 
    for(int i = 0; i < allMeshes.size(); i++) { 
        delete allMeshes.at(i); 
    } 
    allMeshes.clear(); 
    renderMeshes.clear();
 
    allFragments.clear(); 
} 
 
void PotatoForwardEngine::mergeFragments(vector<Fragment> &fragList, Image<Vec3f> *drawBuffer) { 
    // For now, just blindly write all fragments to buffer 
    for(int i = 0; i < fragList.size(); i++) { 
        Fragment f = fragList.at(i); 
        if (drawBuffer->getIndex(f.pos.x, f.pos.y) >= drawBuffer->size()) {
            cerr << "Pos surpassed bounds: (" << f.pos.x << ", " << f.pos.y << ")" << endl;
        }
        else {
            drawBuffer->setPixel(f.pos.x, f.pos.y, Vec3f(f.color)); 
        }
    } 
} 
 
void PotatoForwardEngine::renderToDrawBuffer(Image<Vec3f> *drawBuffer) { 
 
    // Clear list of fragments 
    allFragments.clear(); 
 
    for(int i = 0; i < allMeshes.size(); i++) { 
        // PolyMesh *mesh = allMeshes.at(i); 
        PolyMesh *inputMesh = allMeshes.at(i);
        PolyMesh *mesh = renderMeshes.at(i);

        processGeometryOneMesh(inputMesh, Mat4f(1.0f), Mat4f(1.0f), Mat4f(1.0f), mesh);
        cout << "Face count = " << mesh->getFaces().size() << endl;
 
        // Get fragments for inside of polygons 
        fillTriangles(mesh, allFragments); 
 
        // Get fragments for lines 
        drawLines(mesh, allFragments, DRAW_LINES_AS_WIREFRAME); 
    } 
 
    // Merge fragments 
    mergeFragments(allFragments, drawBuffer); 
} 

void PotatoForwardEngine::processGeometryOneMesh(
    PolyMesh *inputMesh, Mat4f &modelMat, 
    Mat4f &viewMat, Mat4f &projMat, PolyMesh *outMesh
    ) {
    vector<int> clippingCodes {};

    for (int i = 0; i < inputMesh->getVertices().size(); i++) {
        Vec4f pos = Vec4f(inputMesh->getVertices().at(i).pos, 1.0f);
        pos = projMat * viewMat * modelMat * pos;

        // Get clipping code
        int cc = getExtendedCohenSutherlandCode(pos, CLIP_LEFT, CLIP_RIGHT, CLIP_BOTTOM, CLIP_TOP, CLIP_NEAR, CLIP_FAR);
        clippingCodes.push_back(cc);

        // This doesn't do anything atm (I don't think?)
        pos.x /= pos.w;
        pos.y /= pos.w;
        pos.z /= pos.w;

        // Update outMesh's Vert
        outMesh->getVertices().at(i).pos = Vec3f(pos);
    }

    vector<Face> inFaces = inputMesh->getFaces();
    vector<Face> outFaces = outMesh->getFaces();
    outFaces.clear();

    for (int i = 0; i < inFaces.size(); i++) {
        vector<unsigned int> indices = inFaces.at(i).indices;
        bool isInside = true;

        for (unsigned int index : indices) {
            isInside = isInside && clippingCodes.at(index);
        }
        if (isInside) {
            outFaces.push_back(inFaces.at(i));
        }
    }

    for (int i = 0; i < outMesh->getVertices().size(); i++) {
        Vert *v = &(outMesh->getVertices().at(i));
        v->pos.x = windowWidth*(v->pos.x + 1.0f) / 2.0f;
        v->pos.y = windowHeight*(v->pos.y + 1.0f) / 2.0f;
    }
}

 
