
#include"FEMalgorithm.h"
#include"OpenGLRenderer.h"

int main(){

    // Import mesh & material properties
    FEMalgorithm model("../exampleFiles/middleEar.msh", "../exampleFiles/materialProperties.txt");

    // Set local coordinates & boundaries
    model.setLocalCoordinates("eardrum", "../exampleFiles/localCoordinates.txt");
    model.setFixedBoundaries("boundaries");
    model.setBoundaryLoad("excitation", 1);
    model.setBoundaryDamping("impedance", 62000);

    // Assemble matrices & solve
    model.assembleGlobalMatrices();
    model.solve();

    // Process data of interest to be rendered
    model.prepareAnimationForOpenGL(32);

    // Render animation
    OpenGLRenderer render;
    render.initWindow("Middle ear deformation", 1024, 1024);
    render.setMaterial(model, "../vertexFragmentShaders.shader", 0.6, 0.9, 0.4, 16);
    
    while (!render.getWindowShouldClose()){
        render.update();
        render.draw();
        render.incrementFrame();
        render.terminateCondition();
    }

    return 0;
    
}