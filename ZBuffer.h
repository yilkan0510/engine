//
// Created by Manasseh on 20/03/2024.
//

#ifndef ENGINE_ZBUFFER_H
#define ENGINE_ZBUFFER_H
#include <vector>
#include <limits>
using namespace std;
class ZBuffer {
    vector<double> container;
    public:
    int width;
    int height;
    ZBuffer(const int width, const int height);
    double get(const int x, const int y);
    void set(const int x,const int y,const double val);
};


#endif //ENGINE_ZBUFFER_H
