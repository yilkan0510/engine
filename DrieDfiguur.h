//
// Created by yilka on 3/1/2024.
//

#ifndef OPDRACHT_3D_LIJNTEKENINGEN_DRIEDFIGUUR_H
#define OPDRACHT_3D_LIJNTEKENINGEN_DRIEDFIGUUR_H
#include <vector>
#include <string>
#include <list>
#include "Vector3D.h"

// Definieer een klasse voor een vlak, bestaande uit punten
class Face {
public:
    std::vector<int> point_indexes;

    Face();

    explicit Face(const std::vector<int> &pointIndexes);

public:
};

// Definieer een klasse voor een 3D figuur
class Figure {
public:
    std::vector<Vector3D> points;

    Figure(const std::vector<Vector3D> &points, const std::vector<Face> &faces);

    Figure();

    std::vector<Face> faces;
    std::vector<double> color;
};

typedef std::list<Figure> Figures3D;


#endif //OPDRACHT_3D_LIJNTEKENINGEN_DRIEDFIGUUR_H
