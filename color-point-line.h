

#ifndef ENGINE_UTILS_H
#define ENGINE_UTILS_H

#include <vector>

class Color {
public:
    double red;
    double green;

    Color(double red, double green, double blue) : red(red), green(green), blue(blue) {}

    Color(): red(0.2), green(0.3), blue(0.8) {}

    double blue;
};

class Point2D {
public:
    double x;
    double y;

    Point2D(double x, double y) : x(x), y(y) {}
};

class Line2D {
public:
    Point2D p1;
    Point2D p2;
    Color color;

    Line2D(const Point2D &p1, const Point2D &p2) : p1(p1), p2(p2) {}

    Line2D(const Point2D &p1, const Point2D &p2, const Color &color) : p1(p1), p2(p2), color(color) {}

};

using Lines2D = std::vector<Line2D>;

#endif //ENGINE_UTILS_H
