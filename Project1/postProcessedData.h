#pragma once
#include <Eigen\Dense>
#include <Eigen\Core>

#include <igl\sort.h>
#include <igl\sortrows.h>

#include"FEMsolver.h"

class postProcessedData {

private:

    Eigen::MatrixXd	allNodes;
    Eigen::MatrixXi allElements;
    Eigen::MatrixXcf allU;

    unsigned int nrOuterElem, frames;
    Eigen::MatrixXi outerFacets;

    Eigen::VectorXf gl_nodes;
    Eigen::MatrixXcf gl_U;

    std::vector<Eigen::MatrixXf> allDispsT, allAbsDisps;
    Eigen::MatrixXf Uabs_t;


    void locateOuterFacets();

    void extractDataOfInterest();

    void complexToTimeDomain_openMP(unsigned int nrFrames);

    void computeAbsoluteDisplacement(Eigen::MatrixXf Ut, Eigen::MatrixXf& Uabs_t_node);

    void createFrames();

    void createOpenGLVectorsForEachFrequency_openMP(int freqID, std::vector< std::vector<float> >& allFrames, std::vector< std::vector<float> >& allColors, std::vector< std::vector<float> >& allNormals);

    void generateColorsNormals(Eigen::VectorXf gl_def_nodes, Eigen::VectorXf Unorm, std::vector<float>& vertexData, std::vector<float>& colorData, std::vector<float>& normalData);

public:

    unsigned int nrFreqs;
    std::vector < std::vector<std::vector<float>> > allFreqMeshes, allFreqColors, allFreqNormals;

    void importResults(FEMsolver& data);

    void extractOuterSurface();

    void animate(unsigned int frames);

};