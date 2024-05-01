//
// Created by yilka on 3/6/2024.
//

#include "Eye_point_transformaties.h"

Matrix Eye_point_transformaties::eyePointTrans(const Vector3D &eyepoint) {
    double theta, phi, r;
    toPolar(eyepoint, theta, phi, r);

    Matrix viewMatrix;
    viewMatrix(1, 1) = -sin(theta);
    viewMatrix(1, 2) = -cos(theta) * cos(phi);
    viewMatrix(1, 3) = cos(theta) * sin(phi);

    viewMatrix(2, 1) = cos(theta);
    viewMatrix(2, 2) = -sin(theta) * cos(phi);
    viewMatrix(2, 3) = sin(theta) * sin(phi);

    viewMatrix(3, 2) = sin(phi);
    viewMatrix(3, 3) = cos(phi);

    viewMatrix(4, 3) = -r;

    return viewMatrix;
}

void Eye_point_transformaties::applyTranformation(Figure &figs, const Matrix &m) {
    for (auto &point : figs.points) {
            point = point * m;
    }
}

void Eye_point_transformaties::toPolar(const Vector3D &point, double &theta, double &phi, double &r) {
    r = sqrt(pow(point.x, 2) + pow(point.y, 2) + pow(point.z, 2));
    theta = atan2(point.y, point.x);
    phi = acos(point.z / r);
}
