//
// Created by yilka on 4/1/2024.
//

#ifndef ENGINE_ZBUFFER_H
#define ENGINE_ZBUFFER_H
#include <vector>
#include <limits>

class ZBuffer {
public:
    std::vector<std::vector<double>> buffer;
    int width, height;

    ZBuffer(int width, int height) : width(width), height(height), buffer(height, std::vector<double>(width, std::numeric_limits<double>::infinity())) {}

    ZBuffer() {}

    // Returns true if the pixel at (x,y) should be updated (new Z-value is closer)
    bool updatePixel(int x, int y, double zValue) {
        if (x >= 0 && x < width && y >= 0 && y < height && zValue < buffer[y][x]) {
            buffer[y][x] = zValue;
            return true;
        }
        return false;
    }

    // Gets the current Z-value of a pixel
    double getPixelZValue(int x, int y) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            return buffer[y][x];
        }
        return std::numeric_limits<double>::infinity(); // Return infinity if out of bounds
    }
};


#endif //ENGINE_ZBUFFER_H
