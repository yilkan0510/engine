//
// Created by yilka on 3/6/2024.
//

#include "Perspectiefprojectie.h"


Lines2D Perspectiefprojectie::doProjectionfig(const Figures3D &figures3D, const double d) {
    Lines2D lines2D;

    for (const Figure &fig : figures3D) {
        for (const auto &line : fig.faces){
            Point2D point1 = doProjection(fig.points[line.point_indexes[0]], d);
            Point2D point2 = doProjection(fig.points[line.point_indexes[1]], d);
            lines2D.push_back(Line2D(point1, point2, Color(fig.color[0], fig.color[1], fig.color[2])));
        }
    }
    return lines2D;
}

Point2D Perspectiefprojectie::doProjection(const Vector3D &point3D, const double d) {

    double x_accent = (d * point3D.x) / -(point3D.z);
    double y_accent = (d * point3D.y) / -(point3D.z);

    return Point2D(x_accent, y_accent);
}


