#include "drawlines.h"
#include <cmath>
#include <algorithm>


img::EasyImage drawlines2D(Lines2D &lines, int size, const std::vector<double> backgroundcolor) {
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

    for (int i = 0; i < lines.size(); i++) {
        image.draw_line(lines[i].p1.x,lines[i].p1.y,lines[i].p2.x,lines[i].p2.y,img::Color(lines[i].color.red*255,lines[i].color.green*255,lines[i].color.blue*255));
    }
    return image;
}