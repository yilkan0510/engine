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

Vector3D Figure::center() const {
    if (points.empty()) return Vector3D::point(0, 0, 0); // Voorkom deling door nul en gebruik de juiste methode om een punt te creëren

    double sumX = 0, sumY = 0, sumZ = 0;
    for (const Vector3D& point : points) {
        sumX += point.x;
        sumY += point.y;
        sumZ += point.z;
    }
    int numPoints = points.size();
    // Gebruik de statische methode point om een nieuwe Vector3D als punt te creëren
    return Vector3D::point(sumX / numPoints, sumY / numPoints, sumZ / numPoints);
}