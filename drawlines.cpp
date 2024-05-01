#include "drawlines.h"
#include <cmath>
#include <algorithm>
#include <sstream>

img::EasyImage drawlines2D(Lines2D &lines, int size, const std::vector<double> backgroundcolor, bool ZbufLijn, bool ZbufDriehoek) {
    // Xmin, Xmax, Ymin, Ymax bepalen

    double Xmin = lines[0].p1.x;
    double Xmax = lines[0].p1.x;
    double Ymin = lines[0].p1.y;
    double Ymax = lines[0].p1.y;

    for (int i = 0; i < lines.size(); i++) { //zal 3 keer doorlopen bij een driehoek (de 3 te verbinden lijnen)
        if (lines[i].p1.x < Xmin) {
            Xmin = lines[i].p1.x;
        }
        if (lines[i].p1.x > Xmax) {
            Xmax = lines[i].p1.x;
        }
        if (lines[i].p1.y < Ymin) {
            Ymin = lines[i].p1.y;
        }
        if (lines[i].p1.y > Ymax) {
            Ymax = lines[i].p1.y;
        }
        if (lines[i].p2.x < Xmin) {
            Xmin = lines[i].p2.x;
        }
        if (lines[i].p2.x > Xmax) {
            Xmax = lines[i].p2.x;
        }
        if (lines[i].p2.y < Ymin) {
            Ymin = lines[i].p2.y;
        }
        if (lines[i].p2.y > Ymax) {
            Ymax = lines[i].p2.y;
        }
    }

    // Bereken de grootte van de Image
    double Xrange = std::abs(Xmax - Xmin);
    double Yrange = std::abs(Ymax - Ymin);

    int imagex = size * (Xrange / std::max(Xrange, Yrange));
    int imagey = size * (Yrange / std::max(Xrange, Yrange));

    // schalen van de tekening
    // schaalfactor d berekenen

    double d = 0.95*(imagex/Xrange);

    // schaalfactor toepassen
    for (int i = 0; i < lines.size(); i++){
        lines[i].p1.x *= d;
        lines[i].p1.y *= d;
        lines[i].p2.x *= d;
        lines[i].p2.y *= d;
    }

    // berekenen van de verschuiving
    double DCx = d*((Xmin + Xmax)/2);
    double DCy = d*((Ymin + Ymax)/2);
    double dx = imagex/2 -DCx;
    double dy = imagey/2 -DCy;

    // verschuiving toepassen
    for (int i = 0; i < lines.size(); i++){
        lines[i].p1.x += dx;
        lines[i].p1.y += dy;
        lines[i].p2.x += dx;
        lines[i].p2.y += dy;
    }

    img::EasyImage image(imagex, imagey, img::Color(backgroundcolor[0]*255, backgroundcolor[1]*255, backgroundcolor[2]*255));

    // tekenen van de lijnen
    if (ZbufLijn == true || ZbufDriehoek == true){
        ZBuffer zbuffer(imagex, imagey);
        for (const auto &line : lines) {
            int x0 = round(line.p1.x);
            int y0 = round(line.p1.y);
            double z0 = line.z1;
            double z1 = line.z2;
            int x1 = round(line.p2.x);
            int y1 = round(line.p2.y);

            draw_zbuf_line(image, zbuffer, x0, y0, z0, x1, y1, z1, img::Color(line.color.red*255, line.color.green*255, line.color.blue*255));
        }
    }
    if (ZbufDriehoek){
        ZBuffer zbuffer(imagex, imagey);
        for (int i = 0; i < lines.size(); i += 3) { // Assuming that every 3 lines form a triangle
            Vector3D A = Vector3D::point(lines[i].p1.x, lines[i].p1.y, lines[i].z1);
            Vector3D B = Vector3D::point(lines[i+1].p1.x, lines[i+1].p1.y, lines[i+1].z1);
            Vector3D C = Vector3D::point(lines[i+2].p1.x, lines[i+2].p1.y, lines[i+2].z1);
            Color color(lines[i].color.red*255, lines[i].color.green*255, lines[i].color.blue*255);

            draw_zbuf_triag(zbuffer, image, A, B, C, d, dx, dy, color);
        }
    } else {
        for (int i = 0; i < lines.size(); i++) {
            image.draw_line(lines[i].p1.x,lines[i].p1.y,lines[i].p2.x,lines[i].p2.y,img::Color(lines[i].color.red*255,lines[i].color.green*255,lines[i].color.blue*255));
        }
    }
    //rond de coordinaten af
    for (int i = 0; i < lines.size(); i++){
        lines[i].p1.x = round(lines[i].p1.x);
        lines[i].p1.y = round(lines[i].p1.y);
        lines[i].p2.x = round(lines[i].p2.x);
        lines[i].p2.y = round(lines[i].p2.y);
    }
    return image;
}


void draw_zbuf_line(img::EasyImage &image, ZBuffer &zbuffer,
                    int x0, int y0, double z0,
                    int x1, int y1, double z1,
                    const img::Color &color) {
    bool steep = std::abs(y1 - y0) > std::abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
        std::swap(z0, z1);
    }
    int dx = x1 - x0;
    int dy = std::abs(y1 - y0);
    int error = dx / 2;
    int ystep = (y0 < y1) ? 1 : -1;
    int y = y0;
    for (int x = x0; x <= x1; x++) {
        double t = (x - x0) / static_cast<double>(dx);
        double z = z0 * (1 - t) + z1 * t;
        if (steep) {
            if (z < zbuffer.getPixelZValue(y, x)) {
                image(y, x) = color;
                zbuffer.updatePixel(y, x, z);
            }
        } else {
            if (z < zbuffer.getPixelZValue(x, y)) {
                image(x, y) = color;
                zbuffer.updatePixel(x, y, z);
            }
        }
        error -= dy;
        if (error < 0) {
            y += ystep;
            error += dx;
        }
    }
}

void draw_zbuf_triag(ZBuffer &zbuffer, img::EasyImage &image,
                     Vector3D const &A, Vector3D const &B, Vector3D const &C,
                     double d, double dx, double dy, Color color) {
    // Compute the reciprocal of Z values for the vertices
    double zReciprocalA = 1.0 / A.z;
    double zReciprocalB = 1.0 / B.z;
    double zReciprocalC = 1.0 / C.z;

    // Calculate the bounding box of the triangle
    double minX = std::min({A.x, B.x, C.x});
    double maxX = std::max({A.x, B.x, C.x});
    double minY = std::min({A.y, B.y, C.y});
    double maxY = std::max({A.y, B.y, C.y});

    // Loop over the pixels in the bounding box
    for (int x = minX; x <= maxX; x++) {
        for (int y = minY; y <= maxY; y++) {
            // Calculate the barycentric coordinates of the pixel
            double lambda1 = ((B.y - C.y) * (x - C.x) + (C.x - B.x) * (y - C.y)) / ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y));
            double lambda2 = ((C.y - A.y) * (x - C.x) + (A.x - C.x) * (y - C.y)) / ((B.y - C.y) * (A.x - C.x) + (C.x - B.x) * (A.y - C.y));
            double lambda3 = 1.0 - lambda1 - lambda2;

            // Check if the pixel is within the triangle
            if (lambda1 >= 0 && lambda1 <= 1 && lambda2 >= 0 && lambda2 <= 1 && lambda3 >= 0 && lambda3 <= 1) {
                // Interpolate the reciprocal Z value for the pixel
                double zReciprocal = lambda1 * zReciprocalA + lambda2 * zReciprocalB + lambda3 * zReciprocalC;

                // Calculate the corrected Z value
                double z = 1.0 / zReciprocal;

                // Check if the pixel is closer than the current value in the Z-buffer
                if (z < zbuffer.getPixelZValue(x, y)) {
                    // Update the Z-buffer
                    zbuffer.updatePixel(x, y, z);

                    // Draw the pixel on the image
                    image(x, y) = img::Color(color.red, color.green, color.blue);
                }
            }
        }
    }
}

