//
// Created by Manasseh on 21/02/2024.
//

#ifndef ENGINE_LINES2D_H
#define ENGINE_LINES2D_H
#include "Color.h"
#include "Point2D.h"
#include <vector>
using namespace std;
class Lines2D {
public:
    Point2D p1;
    Point2D p2;
    Color color;
    Lines2D(const Point2D &p1, const Point2D &p2, const Color &color);

    Lines2D(const Point2D &p1, const Point2D &p2, const Color &color, double z1, double z2);
};


typedef vector<Lines2D> Lines2DArr;
#endif //ENGINE_LINES2D_H
