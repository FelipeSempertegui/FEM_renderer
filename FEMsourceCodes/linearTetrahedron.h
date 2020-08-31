#pragma once
#include <Eigen\Dense>
#include <Eigen\Core>

class linearTetrahedron {
private:
    Eigen::Matrix <double, 3, 12> tetraN1, tetraN2, tetraN3, tetraN4;

    Eigen::Matrix <double, 3, 12> linearTetraShapeFunction(double q1, double q2, double q3, double q4);

    void tetrahedraGaussPoints(Eigen::Matrix <double, 3, 12>& N1, Eigen::Matrix <double, 3, 12>& N2, Eigen::Matrix <double, 3, 12>& N3, Eigen::Matrix <double, 3, 12>& N4);

    Eigen::Matrix <double, 3, 9> linearTriShapeFunction(double q1, double q2, double q3);

public:

    Eigen::Matrix <double, 12, 12> sumTetraGaussPoints;
    Eigen::Matrix <double, 9, 9> sumTriGaussPoints;
    Eigen::Matrix <double, 3, 9> triN1, triN2, triN3;

    void triGaussPoints(Eigen::Matrix <double, 3, 9>& triN1, Eigen::Matrix <double, 3, 9>& triN2, Eigen::Matrix <double, 3, 9>& triN3);

    Eigen::Matrix <double, 12, 12> sumTetrahedronGaussPoints();

    Eigen::Matrix <double, 9, 9> sumTriangleGaussPoints();

};