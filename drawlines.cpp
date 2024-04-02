#include "drawlines.h"
#include <cmath>
#include <algorithm>



img::EasyImage drawlines2D(Lines2D &lines, int size, const std::vector<double> backgroundcolor, bool Zbuf) {
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

    //rond de coordinaten af
    for (int i = 0; i < lines.size(); i++){
        lines[i].p1.x = round(lines[i].p1.x);
        lines[i].p1.y = round(lines[i].p1.y);
        lines[i].p2.x = round(lines[i].p2.x);
        lines[i].p2.y = round(lines[i].p2.y);
    }

    img::EasyImage image(imagex, imagey, img::Color(backgroundcolor[0]*255, backgroundcolor[1]*255, backgroundcolor[2]*255));

    // tekenen van de lijnen
    if (Zbuf) {
        ZBuffer zbuffer(imagex, imagey);

        for (const auto &line : lines) {
            // Prepare line endpoints, taking into account the image scaling and translation
            int x0 = round(line.p1.x);
            int y0 = round(line.p1.y);
            double z0 = line.z1; // Assuming line.z1 is the Z-value for line.p1 after scaling and translation adjustments

            int x1 = round(line.p2.x);
            int y1 = round(line.p2.y);
            double z1 = line.z2; // Assuming line.z2 is the Z-value for line.p2

            // Draw the line with Z-buffering
            draw_zbuf_line(image, zbuffer, x0, y0, z0, x1, y1, z1, img::Color(line.color.red*255, line.color.green*255, line.color.blue*255));
        }
    } else {
        for (int i = 0; i < lines.size(); i++) {
            image.draw_line(lines[i].p1.x,lines[i].p1.y,lines[i].p2.x,lines[i].p2.y,img::Color(lines[i].color.red*255,lines[i].color.green*255,lines[i].color.blue*255));
        }
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
        double t = (x - x0) / static_cast<double>(dx); // Calculate relative position along the line
        double z = z0 * (1 - t) + z1 * t; // Interpolate Z-value based on relative position
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