#include "ZBuffer.h"

bool ZBuffer::updatePixel(int x, int y, double zValue) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        double oldZ = buffer[y][x];
        if (zValue < oldZ) {
            buffer[y][x] = zValue;
            return true;
        }
    }
    return false;
}

double ZBuffer::getPixelZValue(int x, int y) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        return buffer[y][x];
    }
    return std::numeric_limits<double>::infinity(); // Return infinity if out of bounds
}
