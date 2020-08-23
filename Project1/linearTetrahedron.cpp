#include"linearTetrahedron.h"

Eigen::Matrix <double, 3, 12> linearTetrahedron::linearTetraShapeFunction(double q1, double q2, double q3, double q4) {
    Eigen::Matrix <double, 3, 12> N;

    N << q1, 0, 0, q2, 0, 0, q3, 0, 0, q4, 0, 0,
        0, q1, 0, 0, q2, 0, 0, q3, 0, 0, q4, 0,
        0, 0, q1, 0, 0, q2, 0, 0, q3, 0, 0, q4;

    return N;

}

void linearTetrahedron::tetrahedraGaussPoints(Eigen::Matrix <double, 3, 12>& N1, Eigen::Matrix <double, 3, 12>& N2, Eigen::Matrix <double, 3, 12>& N3, Eigen::Matrix <double, 3, 12>& N4) {
    double a = 0.585410196624969, b = 0.138196601125011;
    N1 = linearTetraShapeFunction(a, b, b, b);
    N2 = linearTetraShapeFunction(b, a, b, b);
    N3 = linearTetraShapeFunction(b, b, a, b);
    N4 = linearTetraShapeFunction(b, b, b, a);
}

Eigen::Matrix <double, 3, 9> linearTetrahedron::linearTriShapeFunction(double q1, double q2, double q3) {
    Eigen::Matrix <double, 3, 9> N;

    N << q1, 0, 0, q2, 0, 0, q3, 0, 0,
        0, q1, 0, 0, q2, 0, 0, q3, 0,
        0, 0, q1, 0, 0, q2, 0, 0, q3;

    return N;

}

void linearTetrahedron::triGaussPoints(Eigen::Matrix <double, 3, 9>& triN1, Eigen::Matrix <double, 3, 9>& triN2, Eigen::Matrix <double, 3, 9>& triN3) {
    double a = 0.651084739625981, b = 0.174457630187009;
    triN1 = linearTriShapeFunction(a, b, b);
    triN2 = linearTriShapeFunction(b, a, b);
    triN3 = linearTriShapeFunction(b, b, a);
}


Eigen::Matrix <double, 12, 12> linearTetrahedron::sumTetrahedronGaussPoints() {

    tetrahedraGaussPoints(tetraN1, tetraN2, tetraN3, tetraN4);

    sumTetraGaussPoints = (tetraN1.transpose() * tetraN1 + tetraN2.transpose() * tetraN2 + tetraN3.transpose() * tetraN3 + tetraN4.transpose() * tetraN4).array() / 4;

    return sumTetraGaussPoints;

}

Eigen::Matrix <double, 9, 9> linearTetrahedron::sumTriangleGaussPoints() {

    triGaussPoints(triN1, triN2, triN3);

    sumTriGaussPoints = (triN1.transpose() * triN1 + triN2.transpose() * triN2 + triN3.transpose() * triN3).array() / 3;

    return sumTriGaussPoints;

}

/*
class linearTetrahedron {
private:
    Eigen::Matrix <double, 3, 12> tetraN1, tetraN2, tetraN3, tetraN4;

    Eigen::Matrix <double, 3, 12> linearTetraShapeFunction(double q1, double q2, double q3, double q4) {
        Eigen::Matrix <double, 3, 12> N;

        N << q1, 0, 0, q2, 0, 0, q3, 0, 0, q4, 0, 0,
            0, q1, 0, 0, q2, 0, 0, q3, 0, 0, q4, 0,
            0, 0, q1, 0, 0, q2, 0, 0, q3, 0, 0, q4;

        return N;

    }

    void tetrahedraGaussPoints(Eigen::Matrix <double, 3, 12>& N1, Eigen::Matrix <double, 3, 12>& N2, Eigen::Matrix <double, 3, 12>& N3, Eigen::Matrix <double, 3, 12>& N4) {
        double a = 0.585410196624969, b = 0.138196601125011;
        N1 = linearTetraShapeFunction(a, b, b, b);
        N2 = linearTetraShapeFunction(b, a, b, b);
        N3 = linearTetraShapeFunction(b, b, a, b);
        N4 = linearTetraShapeFunction(b, b, b, a);
    }

    Eigen::Matrix <double, 3, 9> linearTriShapeFunction(double q1, double q2, double q3) {
        Eigen::Matrix <double, 3, 9> N;

        N << q1, 0, 0, q2, 0, 0, q3, 0, 0,
            0, q1, 0, 0, q2, 0, 0, q3, 0,
            0, 0, q1, 0, 0, q2, 0, 0, q3;

        return N;

    }

public:

    Eigen::Matrix <double, 12, 12> sumTetraGaussPoints;
    Eigen::Matrix <double, 9, 9> sumTriGaussPoints;
    Eigen::Matrix <double, 3, 9> triN1, triN2, triN3;

    void triGaussPoints(Eigen::Matrix <double, 3, 9>& triN1, Eigen::Matrix <double, 3, 9>& triN2, Eigen::Matrix <double, 3, 9>& triN3) {
        double a = 0.651084739625981, b = 0.174457630187009;
        triN1 = linearTriShapeFunction(a, b, b);
        triN2 = linearTriShapeFunction(b, a, b);
        triN3 = linearTriShapeFunction(b, b, a);
    }

    Eigen::Matrix <double, 12, 12> sumTetrahedronGaussPoints() {

        tetrahedraGaussPoints(tetraN1, tetraN2, tetraN3, tetraN4);

        sumTetraGaussPoints = (tetraN1.transpose() * tetraN1 + tetraN2.transpose() * tetraN2 + tetraN3.transpose() * tetraN3 + tetraN4.transpose() * tetraN4).array() / 4;

        return sumTetraGaussPoints;

    }

    Eigen::Matrix <double, 9, 9> sumTriangleGaussPoints() {

        triGaussPoints(triN1, triN2, triN3);

        sumTriGaussPoints = (triN1.transpose() * triN1 + triN2.transpose() * triN2 + triN3.transpose() * triN3).array() / 3;

        return sumTriGaussPoints;

    }

};
*/