//
// Created by Manasseh on 21/02/2024.
//

#ifndef ENGINE_POINT2D_H
#define ENGINE_POINT2D_H
#include<list>
#include <limits>
using namespace std;
class Point2D {
public:
    double x;
    double y;
    double zbuf;
    Point2D(double x, double y);

    Point2D(double x, double y, double zbuf);
};

typedef list<Point2D> Points2D;
#endif //ENGINE_POINT2D_H
