//
// Created by Manasseh on 21/02/2024.
//

#include "Point2D.h"

Point2D::Point2D(double x, double y) : x(x), y(y) {
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();
    this->zbuf = posInf;
}

Point2D::Point2D(double x, double y, double zbuf) : x(x), y(y), zbuf(zbuf) {}
