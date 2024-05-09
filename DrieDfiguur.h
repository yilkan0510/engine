//
// Created by yilka on 3/1/2024.
//

#ifndef OPDRACHT_3D_LIJNTEKENINGEN_DRIEDFIGUUR_H
#define OPDRACHT_3D_LIJNTEKENINGEN_DRIEDFIGUUR_H
#include <vector>
#include <string>
#include <list>
#include "vector3d.h"
#include <vector>

class Face {
public:
    std::vector<int> point_indexes;

    void triangulate(std::vector<Face>& faces) const;

    Face();

    explicit Face(const std::vector<int> &pointIndexes);

public:
};

class Figure {
public:
    std::vector<Vector3D> points;

    Figure(const std::vector<Vector3D> &points, const std::vector<Face> &faces);

    Figure();

    std::vector<Face> faces;
    std::vector<double> color;

    void triangulateFaces();

    void addTriangle(int a, int b, int c);
};

typedef std::list<Figure> Figures3D;


#endif //OPDRACHT_3D_LIJNTEKENINGEN_DRIEDFIGUUR_H
