//
// Created by Manasseh on 1/03/2024.
//

#include "Figure.h"

Figure::Figure(const vector<Vector3D> &points, const vector<Face> &faces, const Color &color) : points(points),
                                                                                                faces(faces),
                                                                                                color(color) {}

Figure::Figure() {

}

Figure::Figure(const vector<Vector3D> &points, const vector<Face> &faces): points(points), faces(faces) {

}
