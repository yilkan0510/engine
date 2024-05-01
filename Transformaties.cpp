//
// Created by yilka on 3/6/2024.
//

#include "Transformaties.h"

Matrix Transformaties::scaleFigure(const double scale) {
    Matrix scalingMatrix;
    scalingMatrix(1, 1) = scale;
    scalingMatrix(2, 2) = scale;
    scalingMatrix(3, 3) = scale;
    return scalingMatrix;
}

Matrix Transformaties::rotateX(const double angle) {
    Matrix rotationMatrix;
    double rad = angle * M_PI / 180.0; //graden naar radialen
    rotationMatrix(2, 2) = cos(rad);
    rotationMatrix(2, 3) = sin(rad);
    rotationMatrix(3, 2) = -sin(rad);
    rotationMatrix(3, 3) = cos(rad);
    return rotationMatrix;
}

Matrix Transformaties::rotateY(const double angle) {
    Matrix rotationMatrix;
    double rad = angle * M_PI / 180.0;
    rotationMatrix(1, 1) = cos(rad);
    rotationMatrix(1, 3) = -sin(rad);
    rotationMatrix(3, 1) = sin(rad);
    rotationMatrix(3, 3) = cos(rad);
    return rotationMatrix;
}

Matrix Transformaties::rotateZ(const double angle) {
    Matrix rotationMatrix;
    double rad = angle * M_PI / 180.0; // Convert degrees to radians
    rotationMatrix(1, 1) = cos(rad);
    rotationMatrix(1, 2) = sin(rad);
    rotationMatrix(2, 1) = -sin(rad);
    rotationMatrix(2, 2) = cos(rad);
    return rotationMatrix;
}

Matrix Transformaties::translate(const Vector3D &vector) {
    Matrix translationMatrix;
    translationMatrix(4, 1) = vector.x;
    translationMatrix(4, 2) = vector.y;
    translationMatrix(4, 3) = vector.z;
    return translationMatrix;
}

void Transformaties::applyTransformation(Figure &fig, const Matrix &m) {
    for (Vector3D &p: fig.points) {
        p = p * m; // moet in deze volgorde
    }
}
