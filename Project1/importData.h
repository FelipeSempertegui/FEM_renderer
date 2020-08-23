#pragma once

#include <vector>
#include <map>
#include <string>
#include <fstream>

#include <Eigen\Dense>
#include <Eigen\Core>

struct FEMdomains {
    Eigen::Matrix<std::complex<double>, 6, 6> stiffnessMatrix;
    double rho;
    Eigen::MatrixXi elements;
    std::string coordinates = "cartesianCoordinates";
    Eigen::MatrixXd e1, e2, e3;
};

struct FEMsurfaces {
    Eigen::MatrixXi tri;
    double inputValue;
};


class importData {

private:
    unsigned int nrOfTriDomains, nrOfTetraDomains;
    std::string domainName;
    std::map<std::string, std::vector<unsigned int>> mapTri_temp, mapTetra_temp;
    FEMdomains MAT;
    FEMsurfaces boundaries;
    Eigen::MatrixXd e1_temp, e2_temp, e3_temp;

    Eigen::Matrix<std::complex<double>, 6, 6> buildOrthotropicStiffness(double E, double po, double xi, double eta);

    Eigen::MatrixXi findTetraElements(std::map<std::string, std::vector<unsigned int>> map, std::string domainName);

    Eigen::MatrixXi findTriElements(std::map<std::string, std::vector<unsigned int>> map, std::string domainName);

public:

    Eigen::MatrixXd	nodes;
    std::map<std::string, FEMdomains> mapTetra;
    std::map<std::string, FEMsurfaces> mapTri;
    Eigen::MatrixXi allElements;

    void importGMSH(const char* string);

    void importMaterials(const char* string);

    void importLocalCoordinates(const char* string);

    void transferLocalCoordinates(std::string name);

    void createSurfaceMap(std::string name, std::string boundaryType, double value);

};