//
// Created by yilka on 3/6/2024.
//

#ifndef ENGINE_TRANSFORMATIES_H
#define ENGINE_TRANSFORMATIES_H
#include "vector3d.h"
#include "DrieDfiguur.h"

#define _USE_MATH_DEFINES
#include <cmath>

class Transformaties {
public:
    Matrix scaleFigure(const double scale);

    Matrix rotateX(const double angle);


    Matrix rotateY(const double angle);


    Matrix rotateZ(const double angle);


    Matrix translate(const Vector3D &vector);


    void applyTransformation(Figure &fig, const Matrix &m);
};


#endif //ENGINE_TRANSFORMATIES_H
