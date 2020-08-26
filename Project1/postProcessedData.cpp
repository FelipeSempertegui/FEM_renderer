#include"postProcessedData.h"



void postProcessedData::locateOuterFacets() {

    nrFreqs = allU.cols();

    Eigen::MatrixXi triangle1(4, 3), triangle2(4, 3), triangle3(4, 3), triangle4(4, 3);
    Eigen::MatrixXi facets(4 * allElements.rows(), 3);
    Eigen::MatrixXi sortedCols, sortedRows, index, absDiff;
    std::vector<Eigen::Triplet<int>> PRowList;
    std::vector<int> duplicates;

    triangle1 << 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0;
    triangle2 << 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0;
    triangle3 << 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1;
    triangle4 << 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1;

    facets << allElements * triangle1, allElements* triangle2, allElements* triangle3, allElements* triangle4;

    igl::sort(facets, 0, 1, sortedCols); // sort columns
    igl::sortrows(sortedCols, 1, sortedRows, index); // sort rows

    // Differentiate & find duplicates
    absDiff = (sortedRows.topRows(sortedRows.rows() - 1) - sortedRows.bottomRows(sortedRows.rows() - 1)).array().abs();
    Eigen::VectorXi locationZeros = absDiff.rowwise().sum();


    for (int i = 0; i < locationZeros.rows(); ++i) {
        if (locationZeros(i) == 0) {
            duplicates.push_back(i);
            duplicates.push_back(i + 1);
        }
    }

    Eigen::SparseMatrix<int> pRows(index.rows() - duplicates.size(), index.rows());

    int c = 0, d = 0;
    for (int i = 0; i < index.rows(); ++i) {
        if (d == duplicates.size()) { d--; }
        if (i == duplicates.at(d)) { d++; }
        else {
            PRowList.push_back(Eigen::Triplet<int>(c, i, 1));
            c++;
        }
    }

    pRows.setFromTriplets(PRowList.begin(), PRowList.end());

    Eigen::VectorXi finalIndices = pRows * index; // correct indices of the outer surface

    nrOuterElem = finalIndices.rows();

    outerFacets.resize(nrOuterElem, 3);

    for (int i = 0; i < nrOuterElem; i++) {
        outerFacets.row(i) = facets.row(finalIndices(i));
    }
}

void postProcessedData::extractDataOfInterest() {

    gl_nodes.resize(9 * nrOuterElem);
    gl_U.resize(9 * nrOuterElem, nrFreqs);

    for (int i = 0; i < nrOuterElem; ++i) {
        // for every element
        // node 1
        gl_nodes(9 * i + 0) = allNodes(outerFacets(i, 0), 0);
        gl_nodes(9 * i + 1) = allNodes(outerFacets(i, 0), 1);
        gl_nodes(9 * i + 2) = allNodes(outerFacets(i, 0), 2);
        // node 2
        gl_nodes(9 * i + 3) = allNodes(outerFacets(i, 1), 0);
        gl_nodes(9 * i + 4) = allNodes(outerFacets(i, 1), 1);
        gl_nodes(9 * i + 5) = allNodes(outerFacets(i, 1), 2);
        // node 3
        gl_nodes(9 * i + 6) = allNodes(outerFacets(i, 2), 0);
        gl_nodes(9 * i + 7) = allNodes(outerFacets(i, 2), 1);
        gl_nodes(9 * i + 8) = allNodes(outerFacets(i, 2), 2);

        // displacements

        gl_U.row(9 * i + 0) = allU.row(3 * outerFacets(i, 0) + 0);
        gl_U.row(9 * i + 1) = allU.row(3 * outerFacets(i, 0) + 1);
        gl_U.row(9 * i + 2) = allU.row(3 * outerFacets(i, 0) + 2);

        gl_U.row(9 * i + 3) = allU.row(3 * outerFacets(i, 1) + 0);
        gl_U.row(9 * i + 4) = allU.row(3 * outerFacets(i, 1) + 1);
        gl_U.row(9 * i + 5) = allU.row(3 * outerFacets(i, 1) + 2);

        gl_U.row(9 * i + 6) = allU.row(3 * outerFacets(i, 2) + 0);
        gl_U.row(9 * i + 7) = allU.row(3 * outerFacets(i, 2) + 1);
        gl_U.row(9 * i + 8) = allU.row(3 * outerFacets(i, 2) + 2);
    }

}

void postProcessedData::complexToTimeDomain_openMP(unsigned int nrFrames) {

    frames = nrFrames;
    float deltaPhi = 2.0f / frames * 3.1415926536, phi = 0.0f;
    std::complex<float> imag(0.0f, 1.0f);


    Uabs_t.resize(frames, nrFreqs);
    Eigen::VectorXf phiVec(frames);

    for (int i = 0; i < frames; i++) {
        phiVec(i) = phi;
        phi += deltaPhi;
    }

    size_t* prefix;
    #pragma omp parallel
    {
        int ithread = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        #pragma omp single
        {
            prefix = new size_t[nthreads + 1];
            prefix[0] = 0;
        }

        std::vector<Eigen::MatrixXf> allDispsT_private, allAbsDisps_private;
        Eigen::MatrixXf Uabs_t_node(3 * nrOuterElem, nrFreqs);
        Eigen::MatrixXf Ut(9 * nrOuterElem, nrFreqs);

        #pragma omp for schedule(static) nowait
        for (int i = 0; i < frames; ++i) {

            Ut = (std::exp(imag * phiVec(i)) * gl_U.array()).real();

            computeAbsoluteDisplacement(Ut, Uabs_t_node);

            Uabs_t.row(i) = (Uabs_t_node.colwise()).maxCoeff();

            allDispsT_private.push_back(Ut);
            allAbsDisps_private.push_back(Uabs_t_node);

        }
        prefix[ithread + 1] = allDispsT_private.size();
        #pragma omp barrier
        #pragma omp single 
        {
            for (int i = 1; i < (nthreads + 1); i++) prefix[i] += prefix[i - 1];
            allDispsT.resize(allDispsT.size() + prefix[nthreads]);
            allAbsDisps.resize(allDispsT.size() + prefix[nthreads]);
        }
        std::copy(allDispsT_private.begin(), allDispsT_private.end(), allDispsT.begin() + prefix[ithread]);
        std::copy(allAbsDisps_private.begin(), allAbsDisps_private.end(), allAbsDisps.begin() + prefix[ithread]);
    }
    delete[] prefix;
}

void postProcessedData::computeAbsoluteDisplacement(Eigen::MatrixXf Ut, Eigen::MatrixXf& Uabs_t_node) {

    Eigen::MatrixXf U_node_1(3, nrFreqs), U_node_2(3, nrFreqs), U_node_3(3, nrFreqs);

    for (int j = 0; j < nrOuterElem; ++j) {
        U_node_1 = Ut.block(9 * j + 0, 0, 3, nrFreqs);
        U_node_2 = Ut.block(9 * j + 3, 0, 3, nrFreqs);
        U_node_3 = Ut.block(9 * j + 6, 0, 3, nrFreqs);
        Uabs_t_node.row(3 * j + 0) = U_node_1.colwise().norm();
        Uabs_t_node.row(3 * j + 1) = U_node_2.colwise().norm();
        Uabs_t_node.row(3 * j + 2) = U_node_3.colwise().norm();
    }



}

void postProcessedData::createFrames() {

    std::vector< std::vector<float> > allFrames, allColors, allNormals;

    for (int k = 0; k < gl_U.cols(); ++k) {

        createOpenGLVectorsForEachFrequency_openMP(k, allFrames, allColors, allNormals);

        allFreqMeshes.push_back(allFrames);
        allFreqColors.push_back(allColors);
        allFreqNormals.push_back(allNormals);

    }

}

void postProcessedData::createOpenGLVectorsForEachFrequency_openMP(int freqID, std::vector< std::vector<float> >& allFrames, std::vector< std::vector<float> >& allColors, std::vector< std::vector<float> >& allNormals) {

    allFrames.clear();
    allColors.clear();
    allNormals.clear();

    size_t* prefix;
    #pragma omp parallel
    {
        int ithread = omp_get_thread_num();
        int nthreads = omp_get_num_threads();
        #pragma omp single
        {
            prefix = new size_t[nthreads + 1];
            prefix[0] = 0;
        }

        std::vector< std::vector<float> > allFrames_private, allColors_private, allNormals_private;
        Eigen::VectorXf gl_def_nodes(9 * nrOuterElem);
        Eigen::VectorXf Unorm(nrOuterElem);
        std::vector<float> vertexData, colorData, normalData;

        #pragma omp for schedule(static) nowait
        for (int i = 0; i < frames; ++i) {

            gl_def_nodes = gl_nodes.array() + allDispsT[i].col(freqID).array() / Uabs_t.col(freqID).maxCoeff() * 0.0018;
            Unorm = allAbsDisps[i].col(freqID).array() / Uabs_t(i, freqID);

            generateColorsNormals(gl_def_nodes, Unorm, vertexData, colorData, normalData);

            allFrames_private.push_back(vertexData); //Mesh

            allColors_private.push_back(colorData); //colors

            allNormals_private.push_back(normalData); //normals
        }

        prefix[ithread + 1] = allFrames_private.size();
        #pragma omp barrier
        #pragma omp single 
        {
            for (int i = 1; i < (nthreads + 1); i++) prefix[i] += prefix[i - 1];
            allFrames.resize(allFrames.size() + prefix[nthreads]);
            allColors.resize(allColors.size() + prefix[nthreads]);
            allNormals.resize(allNormals.size() + prefix[nthreads]);
        }
        std::copy(allFrames_private.begin(), allFrames_private.end(), allFrames.begin() + prefix[ithread]);
        std::copy(allColors_private.begin(), allColors_private.end(), allColors.begin() + prefix[ithread]);
        std::copy(allNormals_private.begin(), allNormals_private.end(), allNormals.begin() + prefix[ithread]);
    }
    delete[] prefix;

}

void postProcessedData::generateColorsNormals(Eigen::VectorXf gl_def_nodes, Eigen::VectorXf Unorm, std::vector<float>& vertexData, std::vector<float>& colorData, std::vector<float>& normalData) {

    Eigen::VectorXf gl_colors(9 * nrOuterElem), gl_normals(9 * nrOuterElem);
    gl_colors.setZero();
    gl_normals.setZero();

    int glID0, glID1, glID2;
    float UnormValue;

    Eigen::Vector3f coord1, coord2, coord3, n;

    for (int j = 0; j < nrOuterElem; ++j) {

        // Set normals ===============================
        coord1 << gl_def_nodes(9 * j + 0), gl_def_nodes(9 * j + 1), gl_def_nodes(9 * j + 2);
        coord2 << gl_def_nodes(9 * j + 3), gl_def_nodes(9 * j + 4), gl_def_nodes(9 * j + 5);
        coord3 << gl_def_nodes(9 * j + 6), gl_def_nodes(9 * j + 7), gl_def_nodes(9 * j + 8);

        n = (coord2 - coord1).cross(coord3 - coord1);
        n.normalize();

        for (int l = 0; l < 3; l++) {

            glID0 = 9 * j + (3 * l + 0);
            glID1 = 9 * j + (3 * l + 1);
            glID2 = 9 * j + (3 * l + 2);

            UnormValue = Unorm(3 * j + l);

            gl_normals(glID0) = n(0);
            gl_normals(glID1) = n(1);
            gl_normals(glID2) = n(2);

            if (UnormValue < 0.35) {
                gl_colors(glID0) = 1 - (1 - 0.5 * std::exp((1 - (0.35 / UnormValue)) / 0.26)); //RED
                gl_colors(glID1) = 1 - (1 - 0.5 * std::exp((1 - (0.7 / UnormValue)) / 0.16)); //GREEN
            }
            else if (UnormValue >= 0.35 && UnormValue < 0.7) {
                gl_colors(glID0) = 1 - (0.5 * std::exp((1 - (UnormValue / 0.35)) / 0.26)); //RED
                gl_colors(glID1) = 1 - (1 - 0.5 * std::exp((1 - (0.7 / UnormValue)) / 0.16)); //GREEN
            }
            else {
                gl_colors(glID0) = 1 - (0.5 * std::exp((1 - (UnormValue / 0.35)) / 0.26)); //RED
                gl_colors(glID1) = 1 - (0.5 * std::exp((1 - (UnormValue / 0.7)) / 0.16)); //GREEN
            }
            gl_colors(glID2) = 0.5 * std::sin(2 * 3.14159265 * UnormValue) + 0.9 * UnormValue; //BLUE
            // if (UnormValue >= 0.7)
        }

    }


    vertexData.assign(gl_def_nodes.data(), gl_def_nodes.data() + gl_def_nodes.size()); //Mesh

    colorData.assign(gl_colors.data(), gl_colors.data() + gl_colors.size()); //colors

    normalData.assign(gl_normals.data(), gl_normals.data() + gl_normals.size()); //normals

}

void postProcessedData::importResults(FEMsolver& data) {

    allNodes = data.nodes;
    allElements = data.allElements;
    allU = (data.displacements).cast < std::complex<float> >();

}

void postProcessedData::extractOuterSurface() {

    this->locateOuterFacets();

    this->extractDataOfInterest();

}

void postProcessedData::animate(unsigned int frames) {

    this->complexToTimeDomain_openMP(frames);

    this->createFrames();

}
