//
// Created by yilka on 3/20/2024.
//
#define _USE_MATH_DEFINES
#include "FigureMaker3D.h"
#include "map"
#include "math.h"



Vector3D calculateMidpoint(const Vector3D& start, const Vector3D& end) {
    return (start + end) * 0.5; // This directly uses the formula without normalization
}
Figure divideInTriangles(Figure &fig) {
    Figure newFig;

    // Copy the existing points to the new figure
    newFig.points = fig.points;

    for (const Face &face : fig.faces) {
        // Get the indices of the vertices of the face
        int a = face.point_indexes[0];
        int b = face.point_indexes[1];
        int c = face.point_indexes[2];

        // Calculate the midpoints for each edge
        Vector3D d = calculateMidpoint(newFig.points[a], newFig.points[b]);
        Vector3D e = calculateMidpoint(newFig.points[a], newFig.points[c]);
        Vector3D f = calculateMidpoint(newFig.points[b], newFig.points[c]);

        // Add the midpoints to the new figure's points vector
        newFig.points.push_back(d);
        newFig.points.push_back(e);
        newFig.points.push_back(f);

        int dIndex = newFig.points.size() - 3;
        int eIndex = newFig.points.size() - 2;
        int fIndex = newFig.points.size() - 1;

        // Create the four new faces using the original points and the new midpoints
        newFig.faces.push_back(Face({a, dIndex, eIndex}));
        newFig.faces.push_back(Face({b, fIndex, dIndex}));
        newFig.faces.push_back(Face({c, eIndex, fIndex}));
        newFig.faces.push_back(Face({dIndex, fIndex, eIndex}));
    }

    return newFig;
}

Figure FigureMaker3D::createIcosahedron() {
    Figure icosahedron;
    icosahedron.points.push_back(Vector3D::point(0, 0, sqrt(5) / 2));
    for (int i = 2; i < 7; ++i) {
        icosahedron.points.push_back(Vector3D::point(cos((i - 2) * 2 * M_PI / 5), sin((i - 2) * 2 * M_PI / 5), 0.5));
    }
    for (int i = 7; i < 12; ++i) {
        icosahedron.points.push_back(Vector3D::point(cos(M_PI / 5 + (i - 7) * 2 * M_PI / 5), sin(M_PI / 5 + (i - 7) * 2 * M_PI / 5), -0.5));
    }
    icosahedron.points.push_back(Vector3D::point(0, 0, -sqrt(5) / 2));

    icosahedron.faces.push_back(Face(std::vector<int>{0, 1, 2}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 6, 10}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 10, 9}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 9, 8}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 8, 7}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 7, 6}));
    icosahedron.faces.push_back(Face(std::vector<int>{1, 10, 6}));
    icosahedron.faces.push_back(Face(std::vector<int>{5, 10, 1}));
    icosahedron.faces.push_back(Face(std::vector<int>{5, 9, 10}));
    icosahedron.faces.push_back(Face(std::vector<int>{4, 9, 5}));
    icosahedron.faces.push_back(Face(std::vector<int>{4, 8, 9}));
    icosahedron.faces.push_back(Face(std::vector<int>{3, 8, 4}));
    icosahedron.faces.push_back(Face(std::vector<int>{3, 7, 8}));
    icosahedron.faces.push_back(Face(std::vector<int>{2, 7, 3}));
    icosahedron.faces.push_back(Face(std::vector<int>{2, 6, 7}));
    icosahedron.faces.push_back(Face(std::vector<int>{1, 6, 2}));
    icosahedron.faces.push_back(Face(std::vector<int>{0, 5, 1}));
    icosahedron.faces.push_back(Face(std::vector<int>{0, 4, 5}));
    icosahedron.faces.push_back(Face(std::vector<int>{0, 3, 4}));
    icosahedron.faces.push_back(Face(std::vector<int>{0, 2, 3}));
    return icosahedron;
}

Figure FigureMaker3D::createTetrahedron() {
    Figure fig;
    fig.points.push_back(Vector3D::point(1, -1, -1));
    fig.points.push_back(Vector3D::point(-1, 1, 1));
    fig.points.push_back(Vector3D::point(1, 1, 1));
    fig.points.push_back(Vector3D::point(-1, -1, 1));

    fig.faces.push_back(Face(std::vector<int>{0, 1, 2}));
    fig.faces.push_back(Face(std::vector<int>{0, 2, 4}));
    fig.faces.push_back(Face(std::vector<int>{0, 3, 1}));
    fig.faces.push_back(Face(std::vector<int>{1, 3, 2}));
    return fig;
}

Figure FigureMaker3D::createSphere(const int n) {
    Figure sphere = createIcosahedron();

    for (int i = 0; i < n; ++i) {
        sphere = divideInTriangles(sphere);
    }
    for (auto& points : sphere.points) {
        points.normalise();
    }
    return sphere;
}

Figure FigureMaker3D::createCone(const int n, const double h) {
    return Figure();
}

Figure FigureMaker3D::createCylinder(const int n, const double h) {
    return Figure();
}

Figure FigureMaker3D::createTorus(const double r, const double R, const int n, const int m) {
    return Figure();
}


