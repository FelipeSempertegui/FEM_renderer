#include"FEMsolver.h"



void FEMsolver::computePermutation(int totalNodes, Eigen::MatrixXi fixedNodes, unsigned int DoF, Eigen::SparseMatrix<double>& Permut, Eigen::SparseMatrix<double>& Permut_t) {

    typedef Eigen::Triplet<double> T;
    Permut.resize(DoF * totalNodes, DoF * (totalNodes - fixedNodes.rows()));
    Permut_t.resize(DoF * (totalNodes - fixedNodes.rows()), DoF * totalNodes);
    std::vector<T> PList;
    int c1 = 0, c2 = 0;
    for (int i = 0; i < totalNodes; i++) {

        if (c1 == fixedNodes.rows()) { c1--; }
        if (i == fixedNodes(c1)) { c1++; }
        else {
            for (int k = 0; k < DoF; k++) {
                PList.push_back(T(3 * i + k, 3 * c2 + k, 1));
            }
            c2++;
        }
    }
    Permut.setFromTriplets(PList.begin(), PList.end());
    Permut_t = Permut.transpose();
}

void FEMsolver::locateTetraElementNodes(Eigen::Vector4i elem, std::vector<unsigned int>& IDs, std::vector<Eigen::Vector3d>& elemNodes) {

    IDs.push_back(elem(0));
    IDs.push_back(elem(1));
    IDs.push_back(elem(2));
    IDs.push_back(elem(3));

    elemNodes.push_back(nodes.row(elem(0)));
    elemNodes.push_back(nodes.row(elem(1)));
    elemNodes.push_back(nodes.row(elem(2)));
    elemNodes.push_back(nodes.row(elem(3)));

}

void FEMsolver::locateTriElementNodes(Eigen::Vector3i elem, std::vector<unsigned int>& IDs, std::vector<Eigen::Vector3d>& elemNodes) {

    IDs.push_back(elem(0));
    IDs.push_back(elem(1));
    IDs.push_back(elem(2));

    elemNodes.push_back(nodes.row(elem(0)));
    elemNodes.push_back(nodes.row(elem(1)));
    elemNodes.push_back(nodes.row(elem(2)));

}

Eigen::Vector3d FEMsolver::computeSurface(std::vector<Eigen::Vector3d> triNodes) {

    Eigen::Vector3d n = ((triNodes.at(1) - triNodes.at(0)).cross(triNodes.at(2) - triNodes.at(0))).array() * 0.5;

    return n;

}

void FEMsolver::strainShapeFunctionVolume(std::vector<Eigen::Vector3d> elemNodes, Eigen::Matrix<double, 6, 12>& B, double& V) {

    Eigen::Vector4d x, y, z;
    Eigen::Matrix4d A, coeffs;

    Eigen::Vector3d n1 = elemNodes.at(0);
    Eigen::Vector3d n2 = elemNodes.at(1);
    Eigen::Vector3d n3 = elemNodes.at(2);
    Eigen::Vector3d n4 = elemNodes.at(3);

    x << n1(0), n2(0), n3(0), n4(0);
    y << n1(1), n2(1), n3(1), n4(1);
    z << n1(2), n2(2), n3(2), n4(2);

    A << Eigen::Vector4d(1, 1, 1, 1), x, y, z;
    coeffs = A.inverse();

    B.setZero();
    for (int i = 0; i < 4; i++)
    {
        B(0, 3 * i + 0) = coeffs(1, i);
        B(1, 3 * i + 1) = coeffs(2, i);
        B(2, 3 * i + 2) = coeffs(3, i);

        B(3, 3 * i + 1) = coeffs(3, i);
        B(3, 3 * i + 2) = coeffs(2, i);

        B(4, 3 * i + 0) = coeffs(3, i);
        B(4, 3 * i + 2) = coeffs(1, i);

        B(5, 3 * i + 0) = coeffs(2, i);
        B(5, 3 * i + 1) = coeffs(1, i);
    }

    V = A.determinant() / 6;
}

void FEMsolver::assembleTetraTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <std::complex<double>, 12, 12> elementMat, std::vector<unsigned int> nodeIDs) {

    typedef Eigen::Triplet<std::complex<double>> T;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 0, 3 * j + 0)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 0, 3 * j + 1)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 0, 3 * j + 2)));

            triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 1, 3 * j + 0)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 1, 3 * j + 1)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 1, 3 * j + 2)));

            triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 2, 3 * j + 0)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 2, 3 * j + 1)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 2, 3 * j + 2)));
        }
    }

}

void FEMsolver::assembleLoadTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <double, 9, 1> elementMat, std::vector<unsigned int> nodeIDs) {

    typedef Eigen::Triplet<std::complex<double>> T;
    for (int i = 0; i < 3; i++) {
        triplets.push_back(T(3 * nodeIDs.at(i) + 0, 0, elementMat(3 * i + 0, 0)));
        triplets.push_back(T(3 * nodeIDs.at(i) + 1, 0, elementMat(3 * i + 1, 0)));
        triplets.push_back(T(3 * nodeIDs.at(i) + 2, 0, elementMat(3 * i + 2, 0)));
    }

}

void FEMsolver::assembleDampingTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <std::complex<double>, 9, 9> elementMat, std::vector<unsigned int> nodeIDs) {

    typedef Eigen::Triplet<std::complex<double>> T;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {

            triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 0, 3 * j + 0)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 0, 3 * j + 1)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 0, 3 * j + 2)));

            triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 1, 3 * j + 0)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 1, 3 * j + 1)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 1, 3 * j + 2)));

            triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 2, 3 * j + 0)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 2, 3 * j + 1)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 2, 3 * j + 2)));

        }
    }

}

void FEMsolver::transformCoordinates(Eigen::Vector3d v1, Eigen::Vector3d v2, Eigen::Vector3d v3, Eigen::Matrix<double, 12, 12>& T) {

    for (int c = 0; c < 4; ++c) {
        T(0 + 3 * c, 0 + 3 * c) = v1[0]; T(0 + 3 * c, 1 + 3 * c) = v1[1]; T(0 + 3 * c, 2 + 3 * c) = v1[2];
        T(1 + 3 * c, 0 + 3 * c) = v2[0]; T(1 + 3 * c, 1 + 3 * c) = v2[1]; T(1 + 3 * c, 2 + 3 * c) = v2[2];
        T(2 + 3 * c, 0 + 3 * c) = v3[0]; T(2 + 3 * c, 1 + 3 * c) = v3[1]; T(2 + 3 * c, 2 + 3 * c) = v3[2];
    }

}

void FEMsolver::rotateNodes(Eigen::Matrix<double, 12, 12> T, std::vector<Eigen::Vector3d>& tetraNodes) {

    Eigen::VectorXd transfNodes(12);

    transfNodes << tetraNodes.at(0), tetraNodes.at(1), tetraNodes.at(2), tetraNodes.at(3);
    transfNodes = T * transfNodes;

    tetraNodes.clear();

    tetraNodes.push_back(transfNodes.segment(0, 3));
    tetraNodes.push_back(transfNodes.segment(3, 3));
    tetraNodes.push_back(transfNodes.segment(6, 3));
    tetraNodes.push_back(transfNodes.segment(9, 3));

}

void FEMsolver::loopThroughDomainsCartesian_openMP(Eigen::MatrixXi elem, Eigen::Matrix <std::complex<double>, 6, 6> stiffMatrix, double rho, std::vector<Eigen::Triplet<std::complex<double>>>& kTrip, std::vector<Eigen::Triplet<std::complex<double>>>& mTrip) {

    linearTetrahedron shapeFunctions;
    Eigen::Matrix <double, 12, 12> N_tetra_gauss = shapeFunctions.sumTetrahedronGaussPoints();
    unsigned int nrDomainElems = elem.rows();

    #pragma omp parallel
    {

        std::vector<unsigned int> nodeIDs;
        std::vector<Eigen::Vector3d> tetraNodes;
        Eigen::Matrix<double, 6, 12> B;
        double Vol;
        Eigen::Matrix<std::complex<double>, 12, 12> k, m;


        std::vector<Eigen::Triplet<std::complex<double>>> kTrip_private;
        std::vector<Eigen::Triplet<std::complex<double>>> mTrip_private;

        #pragma omp for nowait
        for (int i = 0; i < nrDomainElems; i++) {

            locateTetraElementNodes(elem.row(i), nodeIDs, tetraNodes);

            strainShapeFunctionVolume(tetraNodes, B, Vol);

            k = Vol * B.transpose() * stiffMatrix * B;
            m = Vol * rho * N_tetra_gauss;

            assembleTetraTriplets(kTrip_private, k, nodeIDs);
            assembleTetraTriplets(mTrip_private, m, nodeIDs);

            nodeIDs.clear();
            tetraNodes.clear();

        }


        #pragma omp critical
        kTrip.insert(kTrip.end(), kTrip_private.begin(), kTrip_private.end());
        mTrip.insert(mTrip.end(), mTrip_private.begin(), mTrip_private.end());
    }




}

void FEMsolver::loopThroughDomainsLocal_openMP(Eigen::MatrixXd e1, Eigen::MatrixXd e2, Eigen::MatrixXd e3, Eigen::MatrixXi elem, Eigen::Matrix <std::complex<double>, 6, 6> stiffMatrix, double rho, std::vector<Eigen::Triplet<std::complex<double>>>& kTrip, std::vector<Eigen::Triplet<std::complex<double>>>& mTrip) {


    linearTetrahedron shapeFunctions;
    Eigen::Matrix <double, 12, 12> N_tetra_gauss = shapeFunctions.sumTetrahedronGaussPoints();
    unsigned int nrDomainElems = elem.rows();

    #pragma omp parallel
    {

        std::vector<unsigned int> nodeIDs;
        std::vector<Eigen::Vector3d> tetraNodes;
        Eigen::Matrix<double, 6, 12> B;
        double Vol;
        Eigen::Matrix<std::complex<double>, 12, 12> k, m;

        std::vector<Eigen::Triplet<std::complex<double>>> kTrip_private;
        std::vector<Eigen::Triplet<std::complex<double>>> mTrip_private;

        Eigen::Matrix<double, 12, 12> T; T.setZero();

        #pragma omp for nowait //fill vec_private in parallel
        for (int i = 0; i < nrDomainElems; i++) {

            locateTetraElementNodes(elem.row(i), nodeIDs, tetraNodes);

            transformCoordinates(e1.row(i), e2.row(i), e3.row(i), T);

            rotateNodes(T, tetraNodes);

            strainShapeFunctionVolume(tetraNodes, B, Vol);

            k = Vol * T.transpose() * B.transpose() * stiffMatrix * B * T;
            m = Vol * rho * N_tetra_gauss;

            assembleTetraTriplets(kTrip_private, k, nodeIDs);
            assembleTetraTriplets(mTrip_private, m, nodeIDs);

            nodeIDs.clear();
            tetraNodes.clear();

        }

        #pragma omp critical
        kTrip.insert(kTrip.end(), kTrip_private.begin(), kTrip_private.end());
        mTrip.insert(mTrip.end(), mTrip_private.begin(), mTrip_private.end());

    }


}

void FEMsolver::loopThroughBoundaryLoad_openMP(Eigen::MatrixXi elem, double pressure, std::vector<Eigen::Triplet<std::complex<double>>>& fTrip) {


    linearTetrahedron shapeFunctions;
    Eigen::Matrix <double, 3, 9> N1, N2, N3;
    shapeFunctions.triGaussPoints(N1, N2, N3);
    unsigned int nrBoundaryElems = elem.rows();

    #pragma omp parallel
    {
        std::vector<unsigned int> nodeIDs;
        std::vector<Eigen::Vector3d> triNodes;
        Eigen::Vector3d S;
        Eigen::VectorXd f;

        std::vector<Eigen::Triplet<std::complex<double>>> fTrip_private;
        #pragma omp for nowait
        for (int i = 0; i < nrBoundaryElems; i++) {

            locateTriElementNodes(elem.row(i), nodeIDs, triNodes);

            S = computeSurface(triNodes);

            f = ((N1 + N2 + N3).transpose() * S).array() / 3 * pressure;

            assembleLoadTriplets(fTrip_private, f, nodeIDs);

            nodeIDs.clear();
            triNodes.clear();
        }

        #pragma omp critical
        fTrip.insert(fTrip.end(), fTrip_private.begin(), fTrip_private.end());


    }



}

void FEMsolver::loopThroughBoundaryDamping_openMP(Eigen::MatrixXi elem, double damping, std::vector<Eigen::Triplet<std::complex<double>>>& cTrip) {

    linearTetrahedron shapeFunctions;
    Eigen::Matrix <double, 9, 9> N_tri_gauss = shapeFunctions.sumTriangleGaussPoints();

    #pragma omp parallel
    {

        std::vector<unsigned int> nodeIDs;
        std::vector<Eigen::Vector3d> triNodes;
        Eigen::Vector3d S;
        Eigen::Matrix<std::complex<double>, 9, 9> c;

        std::vector<Eigen::Triplet<std::complex<double>>> cTrip_private;
        #pragma omp for nowait //fill vec_private in parallel
        for (int i = 0; i < elem.rows(); i++) {

            locateTriElementNodes(elem.row(i), nodeIDs, triNodes);

            S = computeSurface(triNodes);

            c = damping * S.norm() * N_tri_gauss;

            assembleDampingTriplets(cTrip_private, c, nodeIDs);

            nodeIDs.clear();
            triNodes.clear();
        }

        #pragma omp critical
        cTrip.insert(cTrip.end(), cTrip_private.begin(), cTrip_private.end());

    }




}

void FEMsolver::importMapsNodes(importData& mesh) {
    nodes = mesh.nodes;
    tetraMap = mesh.mapTetra;
    triMap = mesh.mapTri;
    allElements = mesh.allElements;
}

FEMsolver::FEMsolver() {}

FEMsolver::~FEMsolver() {}

void FEMsolver::selectFrequencies() {
    
    int count = 1;
    int ca;

    // array declaration
    double arr[100] = { 0 };

    // prompt user to input numbers & add
    for (int i = 0; i < 100; i++) {
        std::cin >> arr[i];
        ca = std::cin.get();
        // if user presses enter, skip to outputting sum without waiting for 100 values
        if (ca == '\n') { break; }
        count++;
    }

    freq.resize(count);
    for (int i = 0; i < count; ++i) { freq(i) = arr[i]; }

}

void FEMsolver::assembleTetrahedralElements() {

    std::map<std::string, FEMdomains>::iterator it = tetraMap.begin();
    std::vector<Eigen::Triplet<std::complex<double>>> kTriplets, mTriplets;
    kTriplets.reserve(allElements.rows() * 144); mTriplets.reserve(allElements.rows() * 144);
    K.resize(3 * nodes.rows(), 3 * nodes.rows());
    M.resize(3 * nodes.rows(), 3 * nodes.rows());

    while (it != tetraMap.end()) {

        Eigen::MatrixXi tetraElements = (it->second).elements;
        Eigen::Matrix <std::complex<double>, 6, 6> Ctensor = (it->second).stiffnessMatrix;
        double rho = (it->second).rho;
        std::string coordinates = it->second.coordinates;

        if (coordinates == "cartesianCoordinates") {
            this->loopThroughDomainsCartesian_openMP(tetraElements, Ctensor, rho, kTriplets, mTriplets);
        }

        if (coordinates == "localCoordinates") {
            Eigen::MatrixXd e1 = it->second.e1;
            Eigen::MatrixXd e2 = it->second.e2;
            Eigen::MatrixXd e3 = it->second.e3;
            this->loopThroughDomainsLocal_openMP(e1, e2, e3, tetraElements, Ctensor, rho, kTriplets, mTriplets);
        }

        it++;

    }

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            K.setFromTriplets(kTriplets.begin(), kTriplets.end());
            #pragma omp section
            M.setFromTriplets(mTriplets.begin(), mTriplets.end());
        }
    }

}

void FEMsolver::assembleBoundaries() {

    std::map<std::string, FEMsurfaces>::iterator it = triMap.begin();

    while (it != triMap.end()) {

        if (it->first == "boundaryLoad") {

            F.resize(3 * nodes.rows(), 1);
            Eigen::MatrixXi tri = (it->second).tri;
            std::vector<Eigen::Triplet<std::complex<double>>> fTriplets; fTriplets.reserve(tri.rows() * 9);
            double pressure = (it->second).inputValue;

            this->loopThroughBoundaryLoad_openMP(tri, pressure, fTriplets);

            F.setFromTriplets(fTriplets.begin(), fTriplets.end());

        }

        if (it->first == "damping") {

            C.resize(3 * nodes.rows(), 3 * nodes.rows());
            Eigen::MatrixXi tri = (it->second).tri;
            std::vector<Eigen::Triplet<std::complex<double>>> cTriplets; cTriplets.reserve(tri.rows() * 81);
            double damping = (it->second).inputValue;

            this->loopThroughBoundaryDamping_openMP(tri, damping, cTriplets);

            C.setFromTriplets(cTriplets.begin(), cTriplets.end());

        }

        if (it->first == "fixedBoundary") {

            Eigen::MatrixXi tri = (it->second).tri;
            igl::unique(tri, fixedNodes);
            computePermutation(nodes.rows(), fixedNodes, 3, P, Pt);

        }
        it++;
    }

}

void FEMsolver::applyBoundaryConditions() {

    int constrained = 3 * (nodes.rows() - fixedNodes.rows());
    K_c.resize(constrained, constrained);
    M_c.resize(constrained, constrained);
    C_c.resize(constrained, constrained);
    F_c.resize(constrained, 1);
    A_c.resize(constrained, constrained);

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            K_c = Pt * K * P;
            #pragma omp section
            M_c = Pt * M * P;
            #pragma omp section
            C_c = Pt * C * P;
            #pragma omp section
            F_c = Pt * F;
        }
    }


}

void FEMsolver::solveLinearSystem() {

    std::complex<double> im(0, 1);
    Eigen::VectorXcd U_single_freq(3 * nodes.rows()); U_single_freq.setZero();
    displacements.resize(3 * nodes.rows(), freq.size()); displacements.setZero();
    Eigen::PardisoLU<Eigen::SparseMatrix<std::complex<double>> > solver;

    for (int i = 0; i < freq.size(); ++i) {

        double omega = 2 * 3.14159265358979 * freq(i);
        A_c = -pow(omega, 2) * M_c + im * omega * C_c + K_c;
        A_c.makeCompressed();

        solver.analyzePattern(A_c);
        solver.factorize(A_c);
        Eigen::VectorXcd U_c = solver.solve(F_c);

        U_single_freq = P * U_c;

        displacements.col(i) = U_single_freq;

    }

}




/*
class FEMsolver {

private:
    std::map<std::string, FEMdomains> tetraMap;
    std::map<std::string, FEMsurfaces> triMap;
    Eigen::SparseMatrix<double> P, Pt;
    Eigen::MatrixXi fixedNodes;
    Eigen::VectorXd freq;

    Eigen::SparseMatrix<std::complex<double>> K, M, F, C, K_c, M_c, F_c, C_c, A_c;

    void computePermutation(int totalNodes, Eigen::MatrixXi fixedNodes, unsigned int DoF, Eigen::SparseMatrix<double>& Permut, Eigen::SparseMatrix<double>& Permut_t) {

        typedef Eigen::Triplet<double> T;
        Permut.resize(DoF * totalNodes, DoF * (totalNodes - fixedNodes.rows()));
        Permut_t.resize(DoF * (totalNodes - fixedNodes.rows()), DoF * totalNodes);
        std::vector<T> PList;
        int c1 = 0, c2 = 0;
        for (int i = 0; i < totalNodes; i++) {

            if (c1 == fixedNodes.rows()) { c1--; }
            if (i == fixedNodes(c1)) { c1++; }
            else {
                for (int k = 0; k < DoF; k++) {
                    PList.push_back(T(3 * i + k, 3 * c2 + k, 1));
                }
                c2++;
            }
        }
        Permut.setFromTriplets(PList.begin(), PList.end());
        Permut_t = Permut.transpose();
    }

    void locateTetraElementNodes(Eigen::Vector4i elem, std::vector<unsigned int>& IDs, std::vector<Eigen::Vector3d>& elemNodes) {

        IDs.push_back(elem(0));
        IDs.push_back(elem(1));
        IDs.push_back(elem(2));
        IDs.push_back(elem(3));

        elemNodes.push_back(nodes.row(elem(0)));
        elemNodes.push_back(nodes.row(elem(1)));
        elemNodes.push_back(nodes.row(elem(2)));
        elemNodes.push_back(nodes.row(elem(3)));

    }

    void locateTriElementNodes(Eigen::Vector3i elem, std::vector<unsigned int>& IDs, std::vector<Eigen::Vector3d>& elemNodes) {

        IDs.push_back(elem(0));
        IDs.push_back(elem(1));
        IDs.push_back(elem(2));

        elemNodes.push_back(nodes.row(elem(0)));
        elemNodes.push_back(nodes.row(elem(1)));
        elemNodes.push_back(nodes.row(elem(2)));

    }

    Eigen::Vector3d computeSurface(std::vector<Eigen::Vector3d> triNodes) {

        Eigen::Vector3d n = ((triNodes.at(1) - triNodes.at(0)).cross(triNodes.at(2) - triNodes.at(0))).array() * 0.5;

        return n;

    }

    void strainShapeFunctionVolume(std::vector<Eigen::Vector3d> elemNodes, Eigen::Matrix<double, 6, 12>& B, double& V) {

        Eigen::Vector4d x, y, z;
        Eigen::Matrix4d A, coeffs;

        Eigen::Vector3d n1 = elemNodes.at(0);
        Eigen::Vector3d n2 = elemNodes.at(1);
        Eigen::Vector3d n3 = elemNodes.at(2);
        Eigen::Vector3d n4 = elemNodes.at(3);

        x << n1(0), n2(0), n3(0), n4(0);
        y << n1(1), n2(1), n3(1), n4(1);
        z << n1(2), n2(2), n3(2), n4(2);

        A << Eigen::Vector4d(1, 1, 1, 1), x, y, z;
        coeffs = A.inverse();

        B.setZero();
        for (int i = 0; i < 4; i++)
        {
            B(0, 3 * i + 0) = coeffs(1, i);
            B(1, 3 * i + 1) = coeffs(2, i);
            B(2, 3 * i + 2) = coeffs(3, i);

            B(3, 3 * i + 1) = coeffs(3, i);
            B(3, 3 * i + 2) = coeffs(2, i);

            B(4, 3 * i + 0) = coeffs(3, i);
            B(4, 3 * i + 2) = coeffs(1, i);

            B(5, 3 * i + 0) = coeffs(2, i);
            B(5, 3 * i + 1) = coeffs(1, i);
        }

        V = A.determinant() / 6;
    }

    void assembleTetraTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <std::complex<double>, 12, 12> elementMat, std::vector<unsigned int> nodeIDs) {

        typedef Eigen::Triplet<std::complex<double>> T;

        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 0, 3 * j + 0)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 0, 3 * j + 1)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 0, 3 * j + 2)));

                triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 1, 3 * j + 0)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 1, 3 * j + 1)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 1, 3 * j + 2)));

                triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 2, 3 * j + 0)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 2, 3 * j + 1)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 2, 3 * j + 2)));
            }
        }

    }

    void assembleLoadTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <double, 9, 1> elementMat, std::vector<unsigned int> nodeIDs) {

        typedef Eigen::Triplet<std::complex<double>> T;
        for (int i = 0; i < 3; i++) {
            triplets.push_back(T(3 * nodeIDs.at(i) + 0, 0, elementMat(3 * i + 0, 0)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 1, 0, elementMat(3 * i + 1, 0)));
            triplets.push_back(T(3 * nodeIDs.at(i) + 2, 0, elementMat(3 * i + 2, 0)));
        }

    }

    void assembleDampingTriplets(std::vector<Eigen::Triplet<std::complex<double>> >& triplets, Eigen::Matrix <std::complex<double>, 9, 9> elementMat, std::vector<unsigned int> nodeIDs) {

        typedef Eigen::Triplet<std::complex<double>> T;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {

                triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 0, 3 * j + 0)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 0, 3 * j + 1)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 0, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 0, 3 * j + 2)));

                triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 1, 3 * j + 0)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 1, 3 * j + 1)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 1, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 1, 3 * j + 2)));

                triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 0, elementMat(3 * i + 2, 3 * j + 0)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 1, elementMat(3 * i + 2, 3 * j + 1)));
                triplets.push_back(T(3 * nodeIDs.at(i) + 2, 3 * nodeIDs.at(j) + 2, elementMat(3 * i + 2, 3 * j + 2)));

            }
        }

    }

    void transformCoordinates(Eigen::Vector3d v1, Eigen::Vector3d v2, Eigen::Vector3d v3, Eigen::Matrix<double, 12, 12>& T) {

        for (int c = 0; c < 4; ++c) {
            T(0 + 3 * c, 0 + 3 * c) = v1[0]; T(0 + 3 * c, 1 + 3 * c) = v1[1]; T(0 + 3 * c, 2 + 3 * c) = v1[2];
            T(1 + 3 * c, 0 + 3 * c) = v2[0]; T(1 + 3 * c, 1 + 3 * c) = v2[1]; T(1 + 3 * c, 2 + 3 * c) = v2[2];
            T(2 + 3 * c, 0 + 3 * c) = v3[0]; T(2 + 3 * c, 1 + 3 * c) = v3[1]; T(2 + 3 * c, 2 + 3 * c) = v3[2];
        }

    }

    void rotateNodes(Eigen::Matrix<double, 12, 12> T, std::vector<Eigen::Vector3d>& tetraNodes) {

        Eigen::VectorXd transfNodes(12);

        transfNodes << tetraNodes.at(0), tetraNodes.at(1), tetraNodes.at(2), tetraNodes.at(3);
        transfNodes = T * transfNodes;

        tetraNodes.clear();

        tetraNodes.push_back(transfNodes.segment(0, 3));
        tetraNodes.push_back(transfNodes.segment(3, 3));
        tetraNodes.push_back(transfNodes.segment(6, 3));
        tetraNodes.push_back(transfNodes.segment(9, 3));

    }

    //Requires openMP

    void loopThroughDomainsCartesian_openMP(Eigen::MatrixXi elem, Eigen::Matrix <std::complex<double>, 6, 6> stiffMatrix, double rho, std::vector<Eigen::Triplet<std::complex<double>>>& kTrip, std::vector<Eigen::Triplet<std::complex<double>>>& mTrip) {

        linearTetrahedron shapeFunctions;
        Eigen::Matrix <double, 12, 12> N_tetra_gauss = shapeFunctions.sumTetrahedronGaussPoints();
        unsigned int nrDomainElems = elem.rows();

#pragma omp parallel
        {

            std::vector<unsigned int> nodeIDs;
            std::vector<Eigen::Vector3d> tetraNodes;
            Eigen::Matrix<double, 6, 12> B;
            double Vol;
            Eigen::Matrix<std::complex<double>, 12, 12> k, m;


            std::vector<Eigen::Triplet<std::complex<double>>> kTrip_private;
            std::vector<Eigen::Triplet<std::complex<double>>> mTrip_private;

#pragma omp for nowait //fill vec_private in parallel
            for (int i = 0; i < nrDomainElems; i++) {

                locateTetraElementNodes(elem.row(i), nodeIDs, tetraNodes);

                strainShapeFunctionVolume(tetraNodes, B, Vol);

                k = Vol * B.transpose() * stiffMatrix * B;
                m = Vol * rho * N_tetra_gauss;

                assembleTetraTriplets(kTrip_private, k, nodeIDs);
                assembleTetraTriplets(mTrip_private, m, nodeIDs);

                nodeIDs.clear();
                tetraNodes.clear();

            }


#pragma omp critical
            kTrip.insert(kTrip.end(), kTrip_private.begin(), kTrip_private.end());
            mTrip.insert(mTrip.end(), mTrip_private.begin(), mTrip_private.end());
        }




    }

    void loopThroughDomainsLocal_openMP(Eigen::MatrixXd e1, Eigen::MatrixXd e2, Eigen::MatrixXd e3, Eigen::MatrixXi elem, Eigen::Matrix <std::complex<double>, 6, 6> stiffMatrix, double rho, std::vector<Eigen::Triplet<std::complex<double>>>& kTrip, std::vector<Eigen::Triplet<std::complex<double>>>& mTrip) {


        linearTetrahedron shapeFunctions;
        Eigen::Matrix <double, 12, 12> N_tetra_gauss = shapeFunctions.sumTetrahedronGaussPoints();
        unsigned int nrDomainElems = elem.rows();

#pragma omp parallel
        {

            std::vector<unsigned int> nodeIDs;
            std::vector<Eigen::Vector3d> tetraNodes;
            Eigen::Matrix<double, 6, 12> B;
            double Vol;
            Eigen::Matrix<std::complex<double>, 12, 12> k, m;

            std::vector<Eigen::Triplet<std::complex<double>>> kTrip_private;
            std::vector<Eigen::Triplet<std::complex<double>>> mTrip_private;

            Eigen::Matrix<double, 12, 12> T; T.setZero();

#pragma omp for nowait //fill vec_private in parallel
            for (int i = 0; i < nrDomainElems; i++) {

                locateTetraElementNodes(elem.row(i), nodeIDs, tetraNodes);

                transformCoordinates(e1.row(i), e2.row(i), e3.row(i), T);

                rotateNodes(T, tetraNodes);

                strainShapeFunctionVolume(tetraNodes, B, Vol);

                k = Vol * T.transpose() * B.transpose() * stiffMatrix * B * T;
                m = Vol * rho * N_tetra_gauss;

                assembleTetraTriplets(kTrip_private, k, nodeIDs);
                assembleTetraTriplets(mTrip_private, m, nodeIDs);

                nodeIDs.clear();
                tetraNodes.clear();

            }

#pragma omp critical
            kTrip.insert(kTrip.end(), kTrip_private.begin(), kTrip_private.end());
            mTrip.insert(mTrip.end(), mTrip_private.begin(), mTrip_private.end());

        }


    }

    void loopThroughBoundaryLoad_openMP(Eigen::MatrixXi elem, double pressure, std::vector<Eigen::Triplet<std::complex<double>>>& fTrip) {


        linearTetrahedron shapeFunctions;
        Eigen::Matrix <double, 3, 9> N1, N2, N3;
        shapeFunctions.triGaussPoints(N1, N2, N3);
        unsigned int nrBoundaryElems = elem.rows();

#pragma omp parallel
        {
            std::vector<unsigned int> nodeIDs;
            std::vector<Eigen::Vector3d> triNodes;
            Eigen::Vector3d S;
            Eigen::VectorXd f;

            std::vector<Eigen::Triplet<std::complex<double>>> fTrip_private;
#pragma omp for nowait
            for (int i = 0; i < nrBoundaryElems; i++) {

                locateTriElementNodes(elem.row(i), nodeIDs, triNodes);

                S = computeSurface(triNodes);

                f = ((N1 + N2 + N3).transpose() * S).array() / 3 * pressure;

                assembleLoadTriplets(fTrip_private, f, nodeIDs);

                nodeIDs.clear();
                triNodes.clear();
            }

#pragma omp critical
            fTrip.insert(fTrip.end(), fTrip_private.begin(), fTrip_private.end());


        }



    }

    void loopThroughBoundaryDamping_openMP(Eigen::MatrixXi elem, double damping, std::vector<Eigen::Triplet<std::complex<double>>>& cTrip) {

        linearTetrahedron shapeFunctions;
        Eigen::Matrix <double, 9, 9> N_tri_gauss = shapeFunctions.sumTriangleGaussPoints();

#pragma omp parallel
        {

            std::vector<unsigned int> nodeIDs;
            std::vector<Eigen::Vector3d> triNodes;
            Eigen::Vector3d S;
            Eigen::Matrix<std::complex<double>, 9, 9> c;

            std::vector<Eigen::Triplet<std::complex<double>>> cTrip_private;
#pragma omp for nowait //fill vec_private in parallel
            for (int i = 0; i < elem.rows(); i++) {

                locateTriElementNodes(elem.row(i), nodeIDs, triNodes);

                S = computeSurface(triNodes);

                c = damping * S.norm() * N_tri_gauss;

                assembleDampingTriplets(cTrip_private, c, nodeIDs);

                nodeIDs.clear();
                triNodes.clear();
            }

#pragma omp critical
            cTrip.insert(cTrip.end(), cTrip_private.begin(), cTrip_private.end());

        }




    }

public:

    Eigen::MatrixXd	nodes;
    Eigen::MatrixXcd displacements;
    Eigen::MatrixXi allElements;

    FEMsolver() {}
    ~FEMsolver() {}

    void importMapsNodes(importData& mesh) {
        nodes = mesh.nodes;
        tetraMap = mesh.mapTetra;
        triMap = mesh.mapTri;
        allElements = mesh.allElements;
    }

    void selectFrequencies() {
        
        int count = 1;
        int ca;

        // array declaration
        double arr[100] = { 0 };

        // prompt user to input numbers & add
        for (int i = 0; i < 100; i++) {
            std::cin >> arr[i];
            ca = std::cin.get();
            // if user presses enter, skip to outputting sum without waiting for 100 values
            if (ca == '\n') { break; }
            count++;
        }

        freq.resize(count);
        for (int i = 0; i < count; ++i) { freq(i) = arr[i]; }

        //freq.resize(4);
        //freq(0) = 2000;
        //freq(1) = 3000;
        //freq(2) = 5000;
        //freq(3) = 18000;
    }

    void assembleTetrahedralElements() {
        //linha 717 ate 744 --- sem reserve: 137 ms // com reserve: 132 ms
        std::map<std::string, FEMdomains>::iterator it = tetraMap.begin();
        std::vector<Eigen::Triplet<std::complex<double>>> kTriplets, mTriplets;
        kTriplets.reserve(allElements.rows() * 144); mTriplets.reserve(allElements.rows() * 144);
        K.resize(3 * nodes.rows(), 3 * nodes.rows());
        M.resize(3 * nodes.rows(), 3 * nodes.rows());

        while (it != tetraMap.end()) {

            Eigen::MatrixXi tetraElements = (it->second).elements;
            Eigen::Matrix <std::complex<double>, 6, 6> Ctensor = (it->second).stiffnessMatrix;
            double rho = (it->second).rho;
            std::string coordinates = it->second.coordinates;

            if (coordinates == "cartesianCoordinates") {
                this->loopThroughDomainsCartesian_openMP(tetraElements, Ctensor, rho, kTriplets, mTriplets);
            }

            if (coordinates == "localCoordinates") {
                Eigen::MatrixXd e1 = it->second.e1;
                Eigen::MatrixXd e2 = it->second.e2;
                Eigen::MatrixXd e3 = it->second.e3;
                this->loopThroughDomainsLocal_openMP(e1, e2, e3, tetraElements, Ctensor, rho, kTriplets, mTriplets);
            }

            it++;

        }

#pragma omp parallel
        {
#pragma omp sections
            {
#pragma omp section
                K.setFromTriplets(kTriplets.begin(), kTriplets.end());
#pragma omp section
                M.setFromTriplets(mTriplets.begin(), mTriplets.end());
            }
        }

    }

    void assembleBoundaries() {

        std::map<std::string, FEMsurfaces>::iterator it = triMap.begin();

        while (it != triMap.end()) {

            if (it->first == "boundaryLoad") {

                F.resize(3 * nodes.rows(), 1);
                Eigen::MatrixXi tri = (it->second).tri;
                std::vector<Eigen::Triplet<std::complex<double>>> fTriplets; fTriplets.reserve(tri.rows() * 9);
                double pressure = (it->second).inputValue;

                this->loopThroughBoundaryLoad_openMP(tri, pressure, fTriplets);

                F.setFromTriplets(fTriplets.begin(), fTriplets.end());

            }

            if (it->first == "damping") {

                C.resize(3 * nodes.rows(), 3 * nodes.rows());
                Eigen::MatrixXi tri = (it->second).tri;
                std::vector<Eigen::Triplet<std::complex<double>>> cTriplets; cTriplets.reserve(tri.rows() * 81);
                double damping = (it->second).inputValue;

                this->loopThroughBoundaryDamping_openMP(tri, damping, cTriplets);

                C.setFromTriplets(cTriplets.begin(), cTriplets.end());

            }

            if (it->first == "fixedBoundary") {

                Eigen::MatrixXi tri = (it->second).tri;
                igl::unique(tri, fixedNodes);
                computePermutation(nodes.rows(), fixedNodes, 3, P, Pt);

            }
            it++;
        }

    }

    void applyBoundaryConditions() {

        int constrained = 3 * (nodes.rows() - fixedNodes.rows());
        K_c.resize(constrained, constrained);
        M_c.resize(constrained, constrained);
        C_c.resize(constrained, constrained);
        F_c.resize(constrained, 1);
        A_c.resize(constrained, constrained);

#pragma omp parallel
        {
#pragma omp sections
            {
#pragma omp section
                K_c = Pt * K * P;
#pragma omp section
                M_c = Pt * M * P;
#pragma omp section
                C_c = Pt * C * P;
#pragma omp section
                F_c = Pt * F;
            }
        }


    }

    void solveLinearSystem() {

        std::complex<double> im(0, 1);
        Eigen::VectorXcd U_single_freq(3 * nodes.rows()); U_single_freq.setZero();
        displacements.resize(3 * nodes.rows(), freq.size()); displacements.setZero();
        Eigen::PardisoLU<Eigen::SparseMatrix<std::complex<double>> > solver;

        for (int i = 0; i < freq.size(); ++i) {

            double omega = 2 * 3.14159265358979 * freq(i);
            A_c = -pow(omega, 2) * M_c + im * omega * C_c + K_c;
            A_c.makeCompressed();

            solver.analyzePattern(A_c);
            solver.factorize(A_c);
            Eigen::VectorXcd U_c = solver.solve(F_c);

            U_single_freq = P * U_c;

            displacements.col(i) = U_single_freq;

        }

    }

};
*/