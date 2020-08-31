#include"FEMalgorithm.h"



FEMalgorithm::FEMalgorithm(const char* gmshFile, const char* MPFile) {

    std::cout << "1. IMPORTING MESH & MATERIAL PROPERTIES" << std::endl;

    model.importGMSH(gmshFile);
    model.importMaterials(MPFile);

    std::cout << "   - THE MODEL CONTAINS " << 3 * model.nodes.rows() << " DEGREES OF FREEDOM & " << model.allElements.rows() << " ELEMENTS" << "\n" << std::endl;

}

FEMalgorithm::~FEMalgorithm() {}

void FEMalgorithm::setLocalCoordinates(std::string name, const char* localCoordinatesFile) {

    model.importLocalCoordinates(localCoordinatesFile);
    model.transferLocalCoordinates(name);

}

void FEMalgorithm::setFixedBoundaries(std::string surfaceName) {

    model.createSurfaceMap(surfaceName, "fixedBoundary", 0);

}

void FEMalgorithm::setBoundaryLoad(std::string surfaceName, double value) {

    model.createSurfaceMap(surfaceName, "boundaryLoad", value);
}

void FEMalgorithm::setBoundaryDamping(std::string surfaceName, double value) {

    model.createSurfaceMap(surfaceName, "damping", value);
}

void FEMalgorithm::assembleGlobalMatrices() {

    solver.importMapsNodes(model);

    std::cout << "2. ASSEMBLYING GLOBAL MATRICES [MASS (M), DAMPING (C) & STIFFNESS (K)] & LOAD VECTOR (L)" << "\n" << std::endl;
    solver.assembleBoundaries();

    solver.assembleTetrahedralElements();

    std::cout << "3. APPLYING BOUNDARY CONDITIONS" << "\n" << std::endl;
    solver.applyBoundaryConditions();

}

void FEMalgorithm::solve() {

    std::cout << "4. INSERT THE FREQUENCIES [HZ] TO BE EVALUTATED & PRESS 'ENTER' TO PROCEED: ";
    solver.selectFrequencies();

    std::cout << "\n" << "5. SOLVING THE COMPLEX-VALUED LINEAR SYSTEM FOR ALL SELECTED FREQUENCIES" << "\n" << std::endl;
    solver.solveLinearSystem();
}

void FEMalgorithm::prepareAnimationForOpenGL(unsigned int frames) {

    visualizeData.importResults(solver);

    std::cout << "6. RENDERING SOLUTION FOR OPENGL " << std::endl;
    std::cout << "   6.1. EXTRACTING RESULTS FROM THE MODEL'S EXTERNAL SURFACE" << std::endl;

    visualizeData.extractOuterSurface();

    std::cout << "   6.2. CONVERTING THE COMPLEX-VALUED DISPLACEMENTS TO THE TIME-DOMAIN" << std::endl;
    std::cout << "        - DEFORM SURFACE MESH" << std::endl;
    std::cout << "        - COMPUTE RGB COLORS & VERTEX NORMALS FOR THE VERTEX & FRAGMENT SHADERS" << std::endl;
    std::cout << "        - PERFORM THIS OPERATION FOR " << frames << " TIME-STEPS OF A VIBRATIONAL PERIOD & FOR ALL INSERTED FREQUENCIES " << "\n" << std::endl;

    visualizeData.animate(frames);

    allFreqMeshes = visualizeData.allFreqMeshes;
    allFreqColors = visualizeData.allFreqColors;
    allFreqNormals = visualizeData.allFreqNormals;
    this->frames = frames;
    nrFreqs = visualizeData.nrFreqs;

    std::cout << "7. ANIMTED DATA IS READY FOR VISUALIZATION" << std::endl;
    std::cout << "   INSTRUCTIONS: " << std::endl;
    std::cout << "   - ZOOM IN OR OUT: PRESS Q & W" << std::endl;
    std::cout << "   - ROTATE THE MODEL: MOVE YOUR MOUSE" << std::endl;
    std::cout << "   - EXCHANGE AMONG THE DEFORMATION PATTERNS: PRESS 1" << std::endl;
    std::cout << "   - END SESSION: PRESS ESC" << "\n" << std::endl;

    std::cout << "PRESS ENTER TO PROCEED" << std::endl;
    std::cin.get();

}
