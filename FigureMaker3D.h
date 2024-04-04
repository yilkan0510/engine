//
// Created by yilka on 3/20/2024.
//

#ifndef ENGINE_FIGUREMAKER3D_H
#define ENGINE_FIGUREMAKER3D_H
#include "DrieDfiguur.h"


class FigureMaker3D {
public:
    Figure createTetrahedron();

    Figure createIcosahedron();

    Figure createSphere(const int n);

    Figure createCone(const int n, const double h);

    Figure createCylinder(const int n, const double h);

    Figure createTorus(const double r, const double R, const int n, const int m);

    Figure createCube();

    Figure createOctahedron();

    Figure createDodecahedron();

    Figure divideInTriangles(Figure &fig);

};


#endif //ENGINE_FIGUREMAKER3D_H
