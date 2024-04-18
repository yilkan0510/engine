//
// Created by Manasseh on 20/03/2024.
//

#include "ZBuffer.h"


ZBuffer::ZBuffer(const int width, const int height) {
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();
    this->width = width;
    this->height = height;
    container.reserve(height*width);
    for(int i = 0; i < height*width; i++){
        container.push_back(posInf);
    }
}

double ZBuffer::get(const int x, const int y) {
    return container[x*height + y];
}

void ZBuffer::set(const int x, const int y,const double val) {
    container[x*height + y] = val;
}
