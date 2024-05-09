//
// Created by yilka on 3/1/2024.
//

#include "DrieDfiguur.h"

Face::Face(const std::vector<int> &pointIndexes) : point_indexes(pointIndexes) {}

Face::Face() {}

void Face::triangulate(std::vector<Face> &faces) const {
    if (point_indexes.size() < 3) {
        std::cerr << "Face triangulation failed: Face has less than 3 points." << std::endl;
        return;
    }
    // Begin vanaf het tweede punt en creëer driehoeken met het eerste punt en aansluitende paren.
    for (size_t i = 1; i < point_indexes.size() - 1; ++i) {
        faces.push_back(Face({point_indexes[0], point_indexes[i], point_indexes[i + 1]}));
    }
}

Figure::Figure(const std::vector<Vector3D> &points, const std::vector<Face> &faces) : points(points), faces(faces) {}

Figure::Figure() {}

void Figure::triangulateFaces() {
    std::vector<Face> triangulated;
    for (const Face& face : faces) {
        face.triangulate(triangulated);
    }
    faces = std::move(triangulated); // Vervang de faces door de getrianguleerde faces
}

void Figure::addTriangle(int a, int b, int c) {
    faces.push_back(Face({a, b, c}));
}
