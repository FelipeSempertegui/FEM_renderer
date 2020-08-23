#pragma once
#include <string>
#include <vector>
#include<iostream>

#include"importData.h"
#include"FEMsolver.h"
#include"postProcessedData.h"

class FEMalgorithm {

private:
    importData model;
    FEMsolver solver;
    postProcessedData visualizeData;

public:

    unsigned int frames;
    unsigned int nrFreqs;
    std::vector < std::vector<std::vector<float>> > allFreqMeshes, allFreqColors, allFreqNormals;

    FEMalgorithm(const char* gmshFile, const char* MPFile);

    ~FEMalgorithm();

    void setLocalCoordinates(std::string name, const char* localCoordinatesFile);

    void setFixedBoundaries(std::string surfaceName);

    void setBoundaryLoad(std::string surfaceName, double value);

    void setBoundaryDamping(std::string surfaceName, double value);

    void assembleGlobalMatrices();

    void solve();

    void prepareAnimationForOpenGL(unsigned int frames);

};
