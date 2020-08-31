#include"importData.h"


Eigen::Matrix<std::complex<double>, 6, 6> importData::buildOrthotropicStiffness(double E, double po, double xi, double eta) {

    double E1 = E;
    double E2 = E1 * xi;
    double E3 = (E1 + E2) / 2;
    double E12 = sqrt(E1 * E2);
    double E23 = sqrt(E2 * E3);
    double E13 = sqrt(E1 * E3);

    Eigen::Matrix <double, 6, 3> D1;
    Eigen::Matrix <double, 6, 3> D2;

    std::complex<double> Re(1, 0);
    std::complex<double> Im(0, 1);
    Eigen::Matrix <double, 6, 6> C;
    Eigen::Matrix <std::complex<double>, 6, 6> complexC;

    D1 <<
        E1 * (1 - po), E12* po, E13* po,
        E12* po, E2* (1 - po), E23* po,
        E13* po, E23* po, E3* (1 - po),
        0, 0, 0,
        0, 0, 0,
        0, 0, 0;
    D1 *= 1 / ((1 + po) * (1 - 2 * po));

    D2 <<
        0, 0, 0,
        0, 0, 0,
        0, 0, 0,
        E23 / 2, 0, 0,
        0, E13 / 2, 0,
        0, 0, E12 / 2;
    D2 *= 1 / (1 + po);

    C << D1, D2;

    complexC = C * (Re + eta * Im);

    return complexC;

}

Eigen::MatrixXi importData::findTetraElements(std::map<std::string, std::vector<unsigned int>> map, std::string domainName) {

    std::vector<unsigned int> indices = map.find(domainName)->second;
    Eigen::MatrixXi elements_temp;
    elements_temp.resize(indices.size() / 4, 4);

    for (int i = 0; i < elements_temp.rows(); i++) {

        elements_temp(i, 0) = indices.at(4 * i + 0);
        elements_temp(i, 1) = indices.at(4 * i + 1);
        elements_temp(i, 2) = indices.at(4 * i + 2);
        elements_temp(i, 3) = indices.at(4 * i + 3);
    }

    return elements_temp.array() - 1;
}

Eigen::MatrixXi importData::findTriElements(std::map<std::string, std::vector<unsigned int>> map, std::string domainName) {

    std::vector<unsigned int> indices = map.find("\"" + domainName + "\"")->second;
    Eigen::MatrixXi elements_temp;
    elements_temp.resize(indices.size() / 3, 3);

    for (int i = 0; i < elements_temp.rows(); i++) {

        elements_temp(i, 0) = indices.at(3 * i + 0);
        elements_temp(i, 1) = indices.at(3 * i + 1);
        elements_temp(i, 2) = indices.at(3 * i + 2);
    }

    return elements_temp.array() - 1;
}

void importData::importGMSH(const char* string) {

    std::string word;
    std::ifstream infile, infile3;
    unsigned int domainNumber, domainOrder, domainCount;

    unsigned int nodesCount, elementCount, elemType, elemTriNumb_temp, elemTetraNumb_temp, dummy;
    Eigen::Vector3i TRI_temp;
    Eigen::Vector4i TETRA_temp;
    std::vector<int> triElements_temp, tetraElements_temp;
    nrOfTriDomains = 0;
    nrOfTetraDomains = 0;


    infile.open(string);
    //Access data=====================
    std::vector<std::string> domainNameVec;
    std::vector<unsigned int> domainNumberVec;
    std::vector<unsigned int> domainOrderVec;
    std::vector<unsigned int> triElementsIndex, tetraElementsIndex;
    while (infile >> word) {
        if (word == "$PhysicalNames") {
            infile >> domainCount;
            for (int i = 0; i < domainCount; i++) {
                infile >> domainOrder >> domainNumber >> domainName;
                domainOrderVec.push_back(domainOrder);
                domainNameVec.push_back(domainName);
                domainNumberVec.push_back(domainNumber);
            }
        }
        if (word == "$Nodes") {
            infile >> nodesCount;
            nodes.resize(nodesCount, 3);
            for (int i = 0; i < nodesCount; i++) {
                infile >> dummy >> nodes(i, 0) >> nodes(i, 1) >> nodes(i, 2);
            }
        }
        if (word == "$Elements") {
            infile >> elementCount;
            for (int i = 0; i < elementCount; i++) {
                infile >> dummy >> elemType;
                if (elemType == 2) {
                    infile >> dummy >> dummy >> elemTriNumb_temp >> TRI_temp(0) >> TRI_temp(1) >> TRI_temp(2);

                    triElements_temp.push_back(TRI_temp(0));
                    triElements_temp.push_back(TRI_temp(1));
                    triElements_temp.push_back(TRI_temp(2));

                    triElementsIndex.push_back(elemTriNumb_temp);
                }
                if (elemType == 4) {
                    infile >> dummy >> dummy >> elemTetraNumb_temp >> TETRA_temp(0) >> TETRA_temp(1) >> TETRA_temp(2) >> TETRA_temp(3);

                    tetraElements_temp.push_back(TETRA_temp(0));
                    tetraElements_temp.push_back(TETRA_temp(1));
                    tetraElements_temp.push_back(TETRA_temp(2));
                    tetraElements_temp.push_back(TETRA_temp(3));

                    tetraElementsIndex.push_back(elemTetraNumb_temp);
                }
            }
        }
    }

    nodes = nodes.array() / 1000;

    allElements.resize(tetraElementsIndex.size(), 4);

    for (int i = 0; i < tetraElementsIndex.size(); i++) {
        allElements(i, 0) = tetraElements_temp.at(4 * i + 0);
        allElements(i, 1) = tetraElements_temp.at(4 * i + 1);
        allElements(i, 2) = tetraElements_temp.at(4 * i + 2);
        allElements(i, 3) = tetraElements_temp.at(4 * i + 3);
    }
    allElements = allElements.array() - 1;

    // separate domains
    std::vector<unsigned int> elemIndex;

    unsigned int triCount = 0, triCount_temp;
    std::vector<unsigned int> triPhysicalCounter;
    triPhysicalCounter.push_back(triCount);

    unsigned int tetraCount = 0, tetraCount_temp;
    std::vector<unsigned int> tetraPhysicalCounter;
    tetraPhysicalCounter.push_back(tetraCount);

    for (int i = 0; i < domainCount; i++) {

        if (domainOrderVec.at(i) == 2) {
            triCount_temp = std::count(triElementsIndex.begin(), triElementsIndex.end(), domainNumberVec.at(i));
            triCount += triCount_temp;
            triPhysicalCounter.push_back(triCount);
            nrOfTriDomains++;
        }

        if (domainOrderVec.at(i) == 3) {
            tetraCount_temp = std::count(tetraElementsIndex.begin(), tetraElementsIndex.end(), domainNumberVec.at(i));
            tetraCount += tetraCount_temp;
            tetraPhysicalCounter.push_back(tetraCount);
            nrOfTetraDomains++;
        }

    }

    for (int i = 0; i < nrOfTriDomains; i++) {
        for (int j = triPhysicalCounter.at(i); j < triPhysicalCounter.at(i + 1); j++) {
            elemIndex.push_back(triElements_temp.at(3 * j + 0));
            elemIndex.push_back(triElements_temp.at(3 * j + 1));
            elemIndex.push_back(triElements_temp.at(3 * j + 2));
        }
        mapTri_temp.insert(std::make_pair(domainNameVec.at(i), elemIndex));
        elemIndex.clear();
    }

    for (int i = 0; i < nrOfTetraDomains; i++) {
        for (int j = tetraPhysicalCounter.at(i); j < tetraPhysicalCounter.at(i + 1); j++) {
            elemIndex.push_back(tetraElements_temp.at(4 * j + 0));
            elemIndex.push_back(tetraElements_temp.at(4 * j + 1));
            elemIndex.push_back(tetraElements_temp.at(4 * j + 2));
            elemIndex.push_back(tetraElements_temp.at(4 * j + 3));
        }
        mapTetra_temp.insert(std::make_pair(domainNameVec.at(i + nrOfTriDomains), elemIndex));
        elemIndex.clear();
    }

}

void importData::importMaterials(const char* string) {
    std::ifstream infile3;
    infile3.open(string);

    Eigen::MatrixXi elements;

    double E, rho, xi, eta;
    for (int i = 0; i < nrOfTetraDomains; i++) {
        infile3 >> domainName >> E >> rho >> xi >> eta;
        MAT.stiffnessMatrix = buildOrthotropicStiffness(E, 0.3, xi, eta);
        MAT.rho = rho;
        MAT.elements = findTetraElements(mapTetra_temp, domainName);
        mapTetra.insert(std::make_pair(domainName, MAT));
    }

}

void importData::importLocalCoordinates(const char* string) {

    std::ifstream infile2;
    unsigned int localCoordCount;

    infile2.open(string);
    infile2 >> localCoordCount;
    e1_temp.resize(localCoordCount, 3);
    e2_temp.resize(localCoordCount, 3);
    e3_temp.resize(localCoordCount, 3);

    for (int i = 0; i < localCoordCount; i++) {
        infile2 >> e1_temp(i, 0) >> e1_temp(i, 1) >> e1_temp(i, 2) >> e2_temp(i, 0) >> e2_temp(i, 1) >> e2_temp(i, 2) >> e3_temp(i, 0) >> e3_temp(i, 1) >> e3_temp(i, 2);
    }

}

void importData::transferLocalCoordinates(std::string name) {

    std::map<std::string, FEMdomains>::iterator it = mapTetra.find("\"" + name + "\"");

    if (it != mapTetra.end())
        (it->second).coordinates = "localCoordinates";
    (it->second).e1 = e1_temp;
    (it->second).e2 = e2_temp;
    (it->second).e3 = e3_temp;

}

void importData::createSurfaceMap(std::string name, std::string boundaryType, double value) {

    boundaries.tri = findTriElements(mapTri_temp, name);
    boundaries.inputValue = value;
    mapTri.insert(std::make_pair(boundaryType, boundaries));

}
