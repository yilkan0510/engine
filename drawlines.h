#ifndef ENGINE_DRAWLINES_H
#define ENGINE_DRAWLINES_H

#include "easy_image.h"
#include "color-point-line.h"
#include "ZBuffer.h"
#include "easy_image.h"
#include "vector3d.h"
#include "Perspectiefprojectie.h"

img::EasyImage drawlines2D(Lines2D &lines, int size, const std::vector<double> backgroundcolor, bool Zbuflijn, bool ZbufDriehoek);

void draw_zbuf_line(img::EasyImage &image, ZBuffer &zbuffer,
                    int x0, int y0, double z0,
                    int x1, int y1, double z1,
                    const img::Color &color);

void draw_zbuf_triag(ZBuffer &zbuffer, img::EasyImage &image,
                     Vector3D const &A, Vector3D const &B, Vector3D const &C,
                     double d, double dx, double dy, Color color);

#endif //ENGINE_DRAWLINES_H
