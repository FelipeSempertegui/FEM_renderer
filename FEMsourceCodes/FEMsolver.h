#pragma once

#define EIGEN_USE_MKL_ALL
#include <Eigen/PardisoSupport>

#include <Eigen\Dense>
#include <Eigen\Sparse>
#include <Eigen\Core>
#include<map>
#include<vector>
#include<omp.h>
#include<iostream>

#include <igl\unique.h>

#include"linearTetrahedron.h"
#include"importData.h"

class FEMsolver {

private:
    std::map<std::string, FEMdomains> tetraMap;
    std::map<std::string, FEMsurfaces> triMap;
    Eigen::SparseMatrix<double> P, Pt;
    Eigen::MatrixXi fixedNodes;
    Eigen::VectorXd freq;

    Eigen::SparseMatrix<std::complex<double>> K, M, F, C, K_c, M_c, F_c, C_c, A_c;

    void computePermutation(int totalNodes, Eigen::MatrixXi fixedNodes, unsigned int DoF, Eigen::SparseMatrix<double>& Permut, Eigen::SparseMatrix<double>& Permut_t);

    void locateTetraElementNodes(Eigen::Vector4i elem, std::vector<unsigned int>& IDs, std::vector<Eigen::Vector3d>& elemNodes);

    void locateTriElementNodes(Eigen::Vector3i elem, std::vector<unsigned int>& IDs, std::vector<Eigen::Vector3d>& elemNodes);

    Eigen::Vector3d computeSurface(std::vector<Eigen::Vector3d> triNodes);

    void strainShapeFunctionVolume(std::vector<Eigen::Vector3d> elemNodes, Eigen::Matrix<double, 6, 12>& B, double& V);

    void assembleTetraTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <std::complex<double>, 12, 12> elementMat, std::vector<unsigned int> nodeIDs);

    void assembleLoadTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <double, 9, 1> elementMat, std::vector<unsigned int> nodeIDs);

    void assembleDampingTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <std::complex<double>, 9, 9> elementMat, std::vector<unsigned int> nodeIDs);

    void transformCoordinates(Eigen::Vector3d v1, Eigen::Vector3d v2, Eigen::Vector3d v3, Eigen::Matrix<double, 12, 12>& T);

    void rotateNodes(Eigen::Matrix<double, 12, 12> T, std::vector<Eigen::Vector3d>& tetraNodes);

    //Requires openMP

    void loopThroughDomainsCartesian_openMP(Eigen::MatrixXi elem, Eigen::Matrix <std::complex<double>, 6, 6> stiffMatrix, double rho, std::vector<Eigen::Triplet<std::complex<double>>>& kTrip, std::vector<Eigen::Triplet<std::complex<double>>>& mTrip);

    void loopThroughDomainsLocal_openMP(Eigen::MatrixXd e1, Eigen::MatrixXd e2, Eigen::MatrixXd e3, Eigen::MatrixXi elem, Eigen::Matrix <std::complex<double>, 6, 6> stiffMatrix, double rho, std::vector<Eigen::Triplet<std::complex<double>>>& kTrip, std::vector<Eigen::Triplet<std::complex<double>>>& mTrip);

    void loopThroughBoundaryLoad_openMP(Eigen::MatrixXi elem, double pressure, std::vector<Eigen::Triplet<std::complex<double>>>& fTrip);

    void loopThroughBoundaryDamping_openMP(Eigen::MatrixXi elem, double damping, std::vector<Eigen::Triplet<std::complex<double>>>& cTrip);

public:

    Eigen::MatrixXd	nodes;
    Eigen::MatrixXcd displacements;
    Eigen::MatrixXi allElements;

    FEMsolver();
    ~FEMsolver();

    void importMapsNodes(importData& mesh);

    void selectFrequencies();

    void assembleTetrahedralElements();

    void assembleBoundaries();

    void applyBoundaryConditions();

    void solveLinearSystem();

};