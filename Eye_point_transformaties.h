//
// Created by yilka on 3/6/2024.
//

#ifndef ENGINE_EYE_POINT_TRANSFORMATIES_H
#define ENGINE_EYE_POINT_TRANSFORMATIES_H
#include "vector3d.h"
#include "DrieDfiguur.h"
#include <cmath>

class Eye_point_transformaties {
public:
    void toPolar(const Vector3D &point, double &theta, double &phi, double &r);

    Matrix eyePointTrans(const Vector3D &eyepoint);

    void applyTranformation(Figure &figs, const Matrix &m);
};


#endif //ENGINE_EYE_POINT_TRANSFORMATIES_H
