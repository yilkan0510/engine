//
// Created by yilka on 3/1/2024.
//

#include "DrieDfiguur.h"

Face::Face(const std::vector<int> &pointIndexes) : point_indexes(pointIndexes) {}

Face::Face() {}

Figure::Figure(const std::vector<Vector3D> &points, const std::vector<Face> &faces) : points(points), faces(faces) {}

Figure::Figure() {}
