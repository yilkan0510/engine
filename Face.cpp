//
// Created by Manasseh on 1/03/2024.
//

#include "Face.h"

Face::Face(const vector<int> &pointIndices) : point_indices(pointIndices) {}

Face::Face() {}

void Face::triangulate(vector<Face> &faces) {
    if (point_indices.size() < 3)
    {
        faces.push_back(*this);
    }

    for (unsigned int i = 1; i <= point_indices.size() - 2; i++)
    {
        faces.push_back(Face({point_indices[0], point_indices[i], point_indices[i + 1]}));
    }
}
