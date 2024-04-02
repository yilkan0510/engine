#ifndef ENGINE_DRAWLINES_H
#define ENGINE_DRAWLINES_H

#include "easy_image.h"
#include "color-point-line.h"
#include "ZBuffer.h"
#include "easy_image.h"

img::EasyImage drawlines2D(Lines2D &lines, int size, const std::vector<double> backgroundcolor, bool Zbuf);

void draw_zbuf_line(img::EasyImage &image, ZBuffer &zbuffer,
                    int x0, int y0, double z0,
                    int x1, int y1, double z1,
                    const img::Color &color);

#endif //ENGINE_DRAWLINES_H
