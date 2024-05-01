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

std::vector<Vector3D> calculateMiddleTriangle(Figure &icosahedron) {
    std::vector<Vector3D> midpoints;

    for (const Face &face : icosahedron.faces) {
        Vector3D midpoint = Vector3D::point(0, 0, 0);
        for (int index : face.point_indexes) {
            midpoint = midpoint + icosahedron.points[index];
        }
        midpoint = midpoint / 3;
        midpoints.push_back(midpoint);
    }
    return midpoints;
}


Figure FigureMaker3D::divideInTriangles(Figure &fig) {
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
    icosahedron.faces.push_back(Face(std::vector<int>{0, 2, 3}));
    icosahedron.faces.push_back(Face(std::vector<int>{0, 3, 4}));
    icosahedron.faces.push_back(Face(std::vector<int>{0, 4, 5}));
    icosahedron.faces.push_back(Face(std::vector<int>{0, 5, 1}));
    icosahedron.faces.push_back(Face(std::vector<int>{1, 6, 2}));
    icosahedron.faces.push_back(Face(std::vector<int>{2, 6, 7}));
    icosahedron.faces.push_back(Face(std::vector<int>{2, 7, 3}));
    icosahedron.faces.push_back(Face(std::vector<int>{3, 7, 8}));
    icosahedron.faces.push_back(Face(std::vector<int>{3, 8, 4}));
    icosahedron.faces.push_back(Face(std::vector<int>{4, 8, 9}));
    icosahedron.faces.push_back(Face(std::vector<int>{4, 9, 5}));
    icosahedron.faces.push_back(Face(std::vector<int>{5, 9, 10}));
    icosahedron.faces.push_back(Face(std::vector<int>{5, 10, 1}));
    icosahedron.faces.push_back(Face(std::vector<int>{1, 10, 6}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 7, 6}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 8, 7}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 9, 8}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 10, 9}));
    icosahedron.faces.push_back(Face(std::vector<int>{11, 6, 10}));
    return icosahedron;
}

Figure FigureMaker3D::createTetrahedron() {
    Figure fig;

    fig.points.push_back(Vector3D::point(1, -1, -1));
    fig.points.push_back(Vector3D::point(-1, 1 , -1));
    fig.points.push_back(Vector3D::point(1, 1, 1));
    fig.points.push_back(Vector3D::point(-1, -1, 1));

    fig.faces.push_back(Face(std::vector<int>{0, 1, 2}));
    fig.faces.push_back(Face(std::vector<int>{1, 3, 2}));
    fig.faces.push_back(Face(std::vector<int>{0, 3, 1}));
    fig.faces.push_back(Face(std::vector<int>{0, 2, 3}));

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
    Figure cone;

    // Add the tip of the cone
    cone.points.push_back(Vector3D::point(0, 0, h));

    // Generate the base circle points
    for (int i = 0; i < n; ++i) {
        double angle = 2 * M_PI * i / n;
        cone.points.push_back(Vector3D::point(cos(angle), sin(angle), 0));
    }

    // Create the side faces
    for (int i = 1; i <= n; ++i) {
        int nextIndex = (i % n) + 1; // To loop back to the first point after the last
        cone.faces.push_back(Face({0, i, nextIndex}));
    }

    // Create the base face
    std::vector<int> baseFaceIndices;
    for (int i = 1; i <= n; ++i) {
        baseFaceIndices.push_back(i);
    }
    cone.faces.push_back(Face(baseFaceIndices));

    return cone;
}


Figure FigureMaker3D::createCylinder(const int n, const double h) {
    Figure cylinder;

    // Generate the base circle points in the XY plane
    for (int i = 0; i < n; ++i) {
        double angle = 2 * M_PI * i / n;
        cylinder.points.push_back(Vector3D::point(cos(angle), sin(angle), 0));
    }

    // Generate the top circle points in the XY plane, but at height h
    for (int i = 0; i < n; ++i) {
        double angle = 2 * M_PI * i / n;
        cylinder.points.push_back(Vector3D::point(cos(angle), sin(angle), h));
    }

    // Create the side faces with 4 points each (forming a single quadrilateral per face)
    for (int i = 0; i < n; ++i) {
        int nextIndex = (i + 1) % n;
        // Quadrilateral face: i, nextIndex, n + nextIndex, n + i
        cylinder.faces.push_back(Face({i, nextIndex, n + nextIndex, n + i}));
    }

    // Create the base face
    std::vector<int> baseFaceIndices;
    for (int i = 0; i < n; ++i) {
        baseFaceIndices.push_back(i);
    }
    cylinder.faces.push_back(Face(baseFaceIndices));

    // Create the top face
    std::vector<int> topFaceIndices;
    for (int i = 0; i < n; ++i) {
        topFaceIndices.push_back(n + i);
    }
    cylinder.faces.push_back(Face(topFaceIndices));

    return cylinder;
}



Figure FigureMaker3D::createTorus(const double r, const double R, const int n, const int m) {
    Figure torus;

    // Generate the points
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < m; i++) {
            double u = (2 * M_PI * i) / n;
            double v = (2 * M_PI * j) / m;
            double x = (R + r * cos(v)) * cos(u);
            double y = (R + r * cos(v)) * sin(u);
            double z = r * sin(v);
            torus.points.push_back(Vector3D::point(x, y, z));
        }
    }

// Generate the faces for the torus
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            int current = i * m + j;
            int nextI = (i + 1) % n;
            int nextJ = (j + 1) % m;
            int nextInCurrentRow = i * m + nextJ;
            int nextInNextRow = nextI * m + j;

            // The quad face
            torus.faces.push_back(Face({current, nextInCurrentRow, nextI * m + nextJ, nextInNextRow}));
        }
    }

    return torus;
}

Figure FigureMaker3D::createCube() {
    Figure cube;

    // Generate the points
    cube.points.push_back(Vector3D::point(1, -1, -1));
    cube.points.push_back(Vector3D::point(-1, 1, -1));
    cube.points.push_back(Vector3D::point(1, 1, 1));
    cube.points.push_back(Vector3D::point(-1, -1, 1));
    cube.points.push_back(Vector3D::point(1, 1, -1));
    cube.points.push_back(Vector3D::point(-1, -1, -1));
    cube.points.push_back(Vector3D::point(1, -1, 1));
    cube.points.push_back(Vector3D::point(-1, 1, 1));

    cube.faces.push_back(Face(std::vector<int>{0, 4, 2, 6}));
    cube.faces.push_back(Face(std::vector<int>{4, 1, 7, 2}));
    cube.faces.push_back(Face(std::vector<int>{1, 5, 3, 7}));
    cube.faces.push_back(Face(std::vector<int>{5, 0, 6, 3}));
    cube.faces.push_back(Face(std::vector<int>{6, 2, 7, 3}));
    cube.faces.push_back(Face(std::vector<int>{0, 5, 1, 4}));




    return cube;
}

Figure FigureMaker3D::createOctahedron() {
    Figure octahedron;

    octahedron.points.push_back(Vector3D::point(1, 0, 0));
    octahedron.points.push_back(Vector3D::point(0, 1, 0));
    octahedron.points.push_back(Vector3D::point(-1, 0, 0));
    octahedron.points.push_back(Vector3D::point(0, -1, 0));
    octahedron.points.push_back(Vector3D::point(0, 0, -1));
    octahedron.points.push_back(Vector3D::point(0, 0, 1));

    octahedron.faces.push_back(Face(std::vector<int>{0, 1, 5}));
    octahedron.faces.push_back(Face(std::vector<int>{1, 2, 5}));
    octahedron.faces.push_back(Face(std::vector<int>{2, 3, 5}));
    octahedron.faces.push_back(Face(std::vector<int>{3, 0, 5}));
    octahedron.faces.push_back(Face(std::vector<int>{1, 0, 4}));
    octahedron.faces.push_back(Face(std::vector<int>{2, 1, 4}));
    octahedron.faces.push_back(Face(std::vector<int>{3, 2, 4}));
    octahedron.faces.push_back(Face(std::vector<int>{0, 3, 4}));

    return octahedron;
}

Figure FigureMaker3D::createDodecahedron() {
    Figure dodecahedron;
    Figure icosahedron = createIcosahedron();
    std::vector<Vector3D> midpoints = calculateMiddleTriangle(icosahedron);

    for (const Vector3D &point : midpoints) {
        dodecahedron.points.push_back(point);
    }

    dodecahedron.faces.push_back(Face({ 0,  1,  2,  3,  4}));
    dodecahedron.faces.push_back(Face({ 0,  5,  6,  7,  1}));
    dodecahedron.faces.push_back(Face({ 1,  7,  8,  9,  2}));
    dodecahedron.faces.push_back(Face({ 2,  9, 10, 11,  3}));
    dodecahedron.faces.push_back(Face({ 3, 11, 12, 13,  4}));
    dodecahedron.faces.push_back(Face({ 4, 13, 14,  5,  0}));
    dodecahedron.faces.push_back(Face({19, 18, 17, 16, 15}));
    dodecahedron.faces.push_back(Face({19, 14, 13, 12, 18}));
    dodecahedron.faces.push_back(Face({18, 12, 11, 10, 17}));
    dodecahedron.faces.push_back(Face({17, 10,  9,  8, 16}));
    dodecahedron.faces.push_back(Face({16,  8,  7,  6, 15}));
    dodecahedron.faces.push_back(Face({15,  6,  5, 14, 19}));

    return dodecahedron;
}




