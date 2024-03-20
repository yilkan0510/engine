//
// Created by yilka on 3/6/2024.
//

#ifndef ENGINE_PERSPECTIEFPROJECTIE_H
#define ENGINE_PERSPECTIEFPROJECTIE_H
#include "vector3d.h"
#include "DrieDfiguur.h"

#define _USE_MATH_DEFINES
#include <cmath>
#include "color-point-line.h"

class Perspectiefprojectie {
public:
    Lines2D doProjectionfig(const Figures3D &figures3D, const double d);

    Point2D doProjection(const Vector3D &point3D, const double d);



};


#endif //ENGINE_PERSPECTIEFPROJECTIE_H
