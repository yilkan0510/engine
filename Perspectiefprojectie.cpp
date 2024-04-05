//
// Created by yilka on 3/6/2024.
//

#include "Perspectiefprojectie.h"


Lines2D Perspectiefprojectie::doProjectionfig(const Figures3D &figures3D, const double d) {
    Lines2D lines2D;

    for (const Figure &fig : figures3D) {
        for (const auto &face : fig.faces) {
            int nPoints = face.point_indexes.size();
            for (int i = 0; i < nPoints; ++i) {
                // Get the start and end points of the line
                Vector3D p1_3D = fig.points[face.point_indexes[i]];
                Vector3D p2_3D = fig.points[face.point_indexes[(i + 1) % nPoints]];

                // Apply perspective projection to each point to get 2D points and Z-values
                double z1 = -p1_3D.z; // Convert to positive z-value
                double z2 = -p2_3D.z; // Convert to positive z-value

                auto projection1 = doProjection(p1_3D, d);
                Point2D point1 = std::get<0>(projection1);

                auto projection2 = doProjection(p2_3D, d);
                Point2D point2 = std::get<0>(projection2);

                // Add the projected line to the list with its Z-values
                lines2D.emplace_back(Line2D(point1, point2, z1, z2, Color(fig.color[0], fig.color[1], fig.color[2])));
            }
        }
    }
    return lines2D;
}


std::tuple<Point2D, double> Perspectiefprojectie::doProjection(const Vector3D &point3D, const double d) {

    double z = -point3D.z;
    double x_accent = (d * point3D.x) / z;
    double y_accent = (d * point3D.y) / z;

    return {Point2D(x_accent, y_accent), z};
}