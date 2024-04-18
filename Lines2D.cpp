//
// Created by Manasseh on 21/02/2024.
//

#include "Lines2D.h"

Lines2D::Lines2D(const Point2D &p1, const Point2D &p2, const Color &color) : p1(p1), p2(p2), color(color) {}

Lines2D::Lines2D(const Point2D &p1, const Point2D &p2, const Color &color, double z1, double z2) : p1(p1), p2(p2),
                                                                                                   color(color) {}
