//
// Created by Manasseh on 1/03/2024.
//

#ifndef ENGINE_HELPERFUNCTIONS_H
#define ENGINE_HELPERFUNCTIONS_H
#include<vector>
#include<string>
#include "l_parser.h"
#include "easy_image.h"
#include "ini_configuration.h"
#include "Lines2D.h"
#include "Point2D.h"
#include "Color.h"
#include <fstream>
#include<cmath>
#include<stack>
#include "vector3d.h"
#include "Figure.h"
#include "ZBuffer.h"
#include <ostream>
#include <sstream>
using namespace std;
struct PolarCoordinates {
    double theta;
    double phi;
    double r;
    PolarCoordinates(double theta1,double phi1,double r1){
        theta = theta1;
        phi = phi1;
        r = r1;
    }
};

img::EasyImage generate3DImage(ini::Configuration &conf);

bool contains(vector<char> &arr,char character);

bool contains(string &arr,char character);

string replaceCharWithRule(char character, string& drawString,const LParser::LSystem2D &l_system,int iteration);

string replaceWithRules(const string &initiator,const LParser::LSystem2D &l_system,int iteration);

img::EasyImage drawLSystem(const ini::Configuration &conf);

Figure draw3DLSystem(const ini::Configuration &conf,const string &figureName);

img::EasyImage draw2DLines(const vector<Lines2D> &lines, int size,const ini::DoubleTuple& backgroundColor);

img::EasyImage draw2DLinesTriag(Figures3D& figures,vector<Lines2D>& lines,int size,const ini::DoubleTuple& backgroundColor);

Matrix scaleFigure(double scale);

Matrix rotateX(double angle);

Matrix rotateY(double angle);

Matrix rotateZ(double angle);

Matrix translate(const Vector3D &vector);

void applyTransformation(Figure &fig,const Matrix &m);

Matrix eyePointTrans(const Vector3D &eyepoint);

void applyTransformation(Figures3D &figs, const Matrix &m);

PolarCoordinates toPolar(const Vector3D &point);

Lines2DArr doProjection(const Figures3D &figures);

Point2D doProjection(const Vector3D &point, double d);

Point2D doProjection(const Vector3D &point, double d,double dx, double dy);

Point2D doProjection2(const Vector3D &point, double d);

Lines2DArr doProjection2(const Figures3D &figures);

Figure createCube();

Figure createOctahedron();

Figure createDodecahedron();

Figure createTetrahedron();

Figure createSphere(int n);

Figure createCone(int n, double h);

Figure createCylinder(int n, double h);

Figure createTorus(double r, double R, int n , int m);

Figure createIcosahedron();

void decrementFaces(vector<Face>& faces);

void divideInTriangles(Figure& triangle,int n);

string replaceCharWithRule3D(const char character, string& drawString,const LParser::LSystem3D &l_system,int iteration);

string replaceWithRules3D(const string &initiator,const LParser::LSystem3D &l_system,int iteration);

void draw_zbuf_line(ZBuffer& zbuffer,img::EasyImage& image,const Point2D& point1, const Point2D& point2,const Color& color);

void draw_zbuf_triag(ZBuffer& zbuffer, img::EasyImage& image, Figure& figure, double d, double dx, double dy, Color color,const ini::DoubleTuple& backgroundColor);

void draw_zbuf_triag(img::EasyImage& image,ZBuffer& zbuffer, const Vector3D &A, const Vector3D& B, const Vector3D &C, double d, double dx, double dy, const ini::DoubleTuple& backgroundColor, Color color);

vector<Face> triangulate(const Face& face);

void triangulate(Figure &figure);

vector<double> getMinMaxValues(const vector<Lines2D>& lines);

double getImageX(vector<double>& extremas, int size);

double getImageY(vector<double>& extremas, int size);

double getdx(vector<double>& extremas, int size);

double getdy(vector<double>& extremas, int size);

void draw(ZBuffer& zBuffer,img::EasyImage& image, const vector<Lines2D> &lines,double d, double dx,double dy, double imageX, double imageY, const ini::DoubleTuple& backgroundColor);

void calculatexLAndxR(long y, Point2D& a, Point2D& b, Point2D& c,long& xL, long& xR);
#endif //ENGINE_HELPERFUNCTIONS_H
