//
// Created by Manasseh on 1/03/2024.
//

#ifndef ENGINE_FIGURE_H
#define ENGINE_FIGURE_H
#include "vector3d.h"
#include "Face.h"
#include "Color.h"
#include<list>
using namespace std;
class Figure {
public:
    vector<Vector3D> points;
    vector<Face> faces;
    Color color;
    Figure(const vector<Vector3D> &points, const vector<Face> &faces, const Color &color);
    Figure();
    Figure(const vector<Vector3D> &points, const vector <Face> &faces);
};

typedef list<Figure> Figures3D;
#endif //ENGINE_FIGURE_H
