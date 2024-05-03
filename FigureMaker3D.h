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

    void generateFractal(Figure& fig, Figures3D& fractal, int nr_iterations, double scale);

    Matrix scaleFigure (double scale);

    Matrix translate(const Vector3D& vector);

    void applyTransformation(Figure& fig, const Matrix& m);

};


#endif //ENGINE_FIGUREMAKER3D_H
