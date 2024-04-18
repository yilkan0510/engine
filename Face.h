//
// Created by Manasseh on 1/03/2024.
//

#ifndef ENGINE_FACE_H
#define ENGINE_FACE_H
#include <vector>
using namespace std;
class Face {
public:
    vector<int> point_indices;
    explicit Face(const vector<int> &pointIndices);
    Face();

    void triangulate(vector<Face>& faces);

};


#endif //ENGINE_FACE_H
