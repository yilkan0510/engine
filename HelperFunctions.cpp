//
// Created by Manasseh on 1/03/2024.
//
#include "HelperFunctions.h"
#define USE_MATH_DEFINES

bool contains(vector<char> &arr,char character){
    for(auto car:arr){
        if(car == character)return true;
    }
    return false;
}
bool contains(string &arr,char character){
    for(auto car:arr){
        if(car == character)return true;
    }
    return false;
}
string replaceCharWithRule(char character, string& drawString,const LParser::LSystem2D &l_system,int iteration){
    if(iteration == l_system.get_nr_iterations())drawString = character;
    if(iteration == 0)return drawString;
    int size = int(drawString.size());
    string tempString = "";
    for(int i = 0; i < size; i++){
        if(drawString[i] == '-')tempString += "-";
        else if(drawString[i] == '+')tempString += "+";
        else if(drawString[i] == '(')tempString += "(";
        else if(drawString[i] == ')')tempString += ")";
        else tempString += l_system.get_replacement(drawString[i]);
    }
    drawString = tempString;
    return replaceCharWithRule(character,drawString,l_system,iteration - 1);
}
string replaceWithRules(const string &initiator,const LParser::LSystem2D &l_system,int iteration){
    string dString = "";
    string drawString = "";
    for(int i = 0; i < int(initiator.size()) ; i++){
        if(initiator[i] == '+')dString += "+";
        else if(initiator[i] == '-')dString += "-";
        else dString += replaceCharWithRule(initiator[i],drawString,l_system,iteration);
        drawString = "";
    }
    return dString;
}

string replaceCharWithRule3D(const char character, string& drawString,const LParser::LSystem3D &l_system,int iteration){
    if(iteration == l_system.get_nr_iterations())drawString = character;
    if(iteration == 0)return drawString;
    int size = int(drawString.size());
    string tempString = "";
    for(int i = 0; i < size; i++){
        if(drawString[i] == '-')tempString += "-";
        else if(drawString[i] == '+')tempString += "+";
        else if(drawString[i] == '(')tempString += "(";
        else if(drawString[i] == ')')tempString += ")";
        else if(drawString[i] == '^')tempString += "^";
        else if(drawString[i] == '&')tempString += "&";
        else if(drawString[i] == '\\')tempString += "\\";
        else if(drawString[i] == '/')tempString += "/";
        else if(drawString[i] == '|')tempString += "|";
        else tempString += l_system.get_replacement(drawString[i]);
    }
    drawString = tempString;
    return replaceCharWithRule3D(character,drawString,l_system,iteration - 1);
}
string replaceWithRules3D(const string &initiator,const LParser::LSystem3D &l_system,int iteration){
    string dString = "";
    string drawString = "";
    for(int i = 0; i < int(initiator.size()) ; i++){
        dString += replaceCharWithRule3D(initiator[i],drawString,l_system,iteration);
        drawString = "";
    }
    return dString;
}

img::EasyImage drawLSystem(const ini::Configuration &conf){
    LParser::LSystem2D l_system;
    ifstream input_stream(conf["2DLSystem"]["inputfile"].as_string_or_die());
    input_stream >> l_system;
    input_stream.close();
    int size = conf["General"]["size"].as_int_or_die();
    ini::DoubleTuple backgroundcolor = conf["General"]["backgroundcolor"].as_double_tuple_or_die();
    double angle = l_system.get_angle();
    vector<char> legalSymbols = {'+','-','(',')','^','&','\\','/','|'};
    legalSymbols.insert(legalSymbols.begin(),l_system.get_alphabet().begin(),l_system.get_alphabet().end());
    string initiator = l_system.get_initiator();
    string drawString = replaceWithRules(initiator,l_system,int(l_system.get_nr_iterations()));
    ini::DoubleTuple  colorTemp = conf["2DLSystem"]["color"].as_double_tuple_or_die();
    Color color(colorTemp[0],colorTemp[1],colorTemp[2]);
    double currentAngle = l_system.get_starting_angle();
    vector<Lines2D> lines;
    Point2D huidigPositie(0,0);
    stack<pair<Point2D,double>> bracketContainer;
    for(auto character:drawString){
        double radian = currentAngle * (2*acos(0.0)/180);
        if(contains(legalSymbols,character)){
            if(character == '-')currentAngle -= angle;
            else if(character == '+')currentAngle += angle;
            else if(character == '(')bracketContainer.push({huidigPositie,currentAngle});
            else if(character == ')'){
                pair<Point2D,double> pointAngle = bracketContainer.top();
                bracketContainer.pop();
                huidigPositie = pointAngle.first;
                currentAngle = pointAngle.second;
            }
            else  {
                if(l_system.draw(character) == 1){
                    Point2D point1(huidigPositie.x,huidigPositie.y);
                    huidigPositie.x += cos(radian);
                    huidigPositie.y += sin(radian);
                    Point2D point2(huidigPositie.x,huidigPositie.y);
                    Lines2D line(point1,point2,color);
                    lines.push_back(line);
                }
                else if(l_system.draw(character) == 0){
                    huidigPositie.x += cos(radian);
                    huidigPositie.y += sin(radian);
                }
            }
        }
    }
    return draw2DLines(lines,size,backgroundcolor);
}

using namespace std;
img::EasyImage draw2DLines(const vector<Lines2D> &lines, int size,const ini::DoubleTuple& backgroundColor){
    if(int(lines.size()) == 0 or size == 0)img::EasyImage image(100,100) ;
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();
    Lines2D line1 = lines[0];
    double xMin = line1.p1.x;
    double xMax = line1.p1.x;
    double yMin = line1.p1.y;
    double yMax = line1.p1.y;
    double xRange = 0;
    double yRange = 0;
    double imageX = 0;
    double imageY = 0;
    double DCx = 0;
    double DCy = 0;
    double dx = 0;
    double dy = 0;
    double d = 0;
    vector<double> extremas = getMinMaxValues(lines);
    xMin = extremas[0];
    xMax = extremas[1];
    yMin = extremas[2];
    yMax = extremas[3];
    xRange = xMax - xMin;
    yRange = yMax - yMin;
    imageX = getImageX(extremas,size);
    imageY = getImageY(extremas,size);
    ZBuffer zBuffer(floor(imageX),floor(imageY));
    d = 0.95 * imageX / xRange;
    dx = getdx(extremas,size);
    dy = getdy(extremas,size);
    img::EasyImage image(floor(imageX),floor(imageY));
    draw(zBuffer,image,lines,d,dx,dy,imageX,imageY,backgroundColor);
    return image;
}

PolarCoordinates toPolar(const Vector3D &point) {
    const double r = sqrt(pow(point.x,2) + pow(point.y,2) + pow(point.z,2));
    const double theta = atan2(point.y,point.x);
    const double phi = acos(point.z/r);
    return PolarCoordinates(theta,phi,r);
}

Point2D doProjection(const Vector3D &point, double d) {
    d = 1;
    return Point2D(d * point.x / -point.z, d * point.y / -point.z);
}

void applyTransformation(Figure &fig, const Matrix &m) {
    for(auto & point : fig.points){
        point *= m;
    }
}

Matrix eyePointTrans(const Vector3D &eyepoint) {
    Matrix V;
    PolarCoordinates polarCor = toPolar(eyepoint);
    V(1,1) = -sin(polarCor.theta);
    V(1,2) = -cos(polarCor.theta)*cos(polarCor.phi);
    V(1,3) = cos(polarCor.theta)*sin(polarCor.phi);
    V(2,1) = cos(polarCor.theta);
    V(2,2) = -sin(polarCor.theta)*cos(polarCor.phi);
    V(2,3) = sin(polarCor.theta)*sin(polarCor.phi);
    V(3,2) = sin(polarCor.phi);
    V(3,3) = cos(polarCor.phi);
    V(4,3) = -polarCor.r;
    return V;
}

Matrix translate(const Vector3D &vector) {
    Matrix V;
    V(4,1) = vector.x;
    V(4,2) = vector.y;
    V(4,3) = vector.z;
    return V;
}

Matrix rotateZ(double angle) {
    Matrix V;
    double radian = angle * 2*acos(0.0)/180;
    V(1,1) = cos(radian);
    V(1,2) = sin(radian);
    V(2,1) = -sin(radian);
    V(2,2) = cos(radian);
    return V;
}

Matrix rotateY(double angle) {
    Matrix V;
    double radian = angle * 2*acos(0.0)/180;
    V(1,1) = cos(radian);
    V(1,3) = -sin(radian);
    V(3,1) = sin(radian);
    V(3,3) = cos(radian);
    return V;
}

Matrix rotateX(double angle) {
    Matrix V;
    double radian = angle * 2*acos(0.0)/180;
    V(2,2) = cos(radian);
    V(2,3) = sin(radian);
    V(3,2) = -sin(radian);
    V(3,3) = cos(radian);
    return V;
}

Matrix scaleFigure(double scale) {
    Matrix V;
    V(1,1) = scale;
    V(2,2) = scale;
    V(3,3) = scale;
    return V;
}

void applyTransformation(Figures3D &figs, const Matrix &m) {
    for(auto &fig:figs){
        applyTransformation(fig,m);
    }
}

Lines2DArr doProjection(const Figures3D &figures) {
    Lines2DArr arr = {};
    for(auto fig:figures){
        for(auto face:fig.faces){
            if(int(face.point_indices.size()) == 2){
                Point2D point1 = doProjection(fig.points[face.point_indices[0]],1);
                Point2D point2 = doProjection(fig.points[face.point_indices[1]],1);
                arr.emplace_back(point1,point2,fig.color);
            }
            else if(int(face.point_indices.size()) > 2){
                for(int i = 0; i < int(face.point_indices.size()); i++){
                    Point2D point1 = doProjection(fig.points[face.point_indices[i]],1);
                    Point2D point2 = doProjection(fig.points[face.point_indices[(i+1)%int(face.point_indices.size())]],1);
                    arr.emplace_back(point1,point2,fig.color);
                }
            }

        }
    }
    return arr;
}

img::EasyImage generate3DImage(ini::Configuration &conf){
    ini::DoubleTuple eyePoint = conf["General"]["eye"].as_double_tuple_or_die();
    ini::DoubleTuple backgroundColor = conf["General"]["backgroundcolor"].as_double_tuple_or_die();
    string type = conf["General"]["type"];
    int nrFigures = conf["General"]["nrFigures"].as_int_or_die();
    int size = conf["General"]["size"].as_int_or_die();
    Figures3D figs;
    for(int i = 0; i < nrFigures; i++){
        Figure figure;
        string figureName = "Figure" + to_string(i);
        Matrix rotateXMatrix = rotateX(conf[figureName]["rotateX"].as_double_or_die());
        Matrix rotateYMatrix = rotateY(conf[figureName]["rotateY"].as_double_or_die());
        Matrix rotateZMatrix = rotateZ(conf[figureName]["rotateZ"].as_double_or_die());
        Matrix scaleMatrix = scaleFigure(conf[figureName]["scale"].as_double_or_die());
        ini::DoubleTuple center = conf[figureName]["center"].as_double_tuple_or_die();
        Matrix translateMatrix = translate(Vector3D::vector(center[0],center[1],center[2]));
        Matrix modelMatrix = scaleMatrix*rotateXMatrix*rotateYMatrix*rotateZMatrix*translateMatrix*eyePointTrans(Vector3D::point(eyePoint[0],eyePoint[1],eyePoint[2])); //rotateXMatrix*rotateZMatrix
        ini::DoubleTuple color = conf[figureName]["color"].as_double_tuple_or_die();
        int nrPoints = 0;
        int nrLines = 0;
        conf[figureName]["nrPoints"].as_int_if_exists(nrPoints);
        conf[figureName]["nrLines"].as_int_if_exists(nrLines);
        if(conf[figureName]["type"].as_string_or_die() == "LineDrawing"){
            figure.color = Color(color[0],color[1],color[2]);
            for(int j = 0; j < nrPoints ; j++){
                string pointName = "point" + to_string(j);
                ini::DoubleTuple point = conf[figureName][pointName].as_double_tuple_or_die();
                Vector3D realPoint = Vector3D::point(point[0],point[1],point[2]);
                figure.points.push_back(realPoint);
            }
            for(int j = 0; j < nrLines; j++){
                string lineName = "line" + to_string(j);
                ini::DoubleTuple line = conf[figureName][lineName].as_double_tuple_or_die();
                Face face({int(line[0]),int(line[1])});
                figure.faces.push_back(face);
            }
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Cube"){
            figure = createCube();
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Tetrahedron"){
            figure = createTetrahedron();
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Octahedron"){
            figure = createOctahedron();
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Icosahedron"){
            figure = createIcosahedron();
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Dodecahedron"){
            figure = createDodecahedron();
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Cone"){
            const int n = conf[figureName]["n"].as_int_or_die();
            const double h = conf[figureName]["height"].as_double_or_die();
            figure = createCone(n,h);
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Cylinder"){
            const int n = conf[figureName]["n"].as_int_or_die();
            const double h = conf[figureName]["height"].as_double_or_die();
            figure = createCylinder(n,h);
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Sphere"){
            const int n = conf[figureName]["n"].as_int_or_die();
            figure = createSphere(n);
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "Torus"){
            const int n = conf[figureName]["n"].as_int_or_die();
            const double r = conf[figureName]["r"].as_double_or_die();
            const int m = conf[figureName]["m"].as_int_or_die();
            const double R = conf[figureName]["R"].as_double_or_die();
            figure = createTorus(r,R,n,m);
            figure.color = Color(color[0],color[1],color[2]);
        }
        else if(conf[figureName]["type"].as_string_or_die() == "3DLSystem"){
            figure = draw3DLSystem(conf,figureName);
            figure.color = Color(color[0],color[1],color[2]);
        }
        else continue;
        if(conf["General"]["type"].as_string_or_die() == "ZBuffering"){
            triangulate(figure);
        }
        applyTransformation(figure,modelMatrix);
        figs.push_back(figure);
    }
    Lines2DArr lines;
    if(conf["General"]["type"].as_string_or_die() == "ZBufferedWireframe" || conf["General"]["type"].as_string_or_die() == "ZBuffering")
         lines = doProjection2(figs);
    else
         lines = doProjection(figs);
    if(lines.empty())
        return img::EasyImage(200,10);
    else
        if(conf["General"]["type"].as_string_or_die() == "ZBuffering")
            return draw2DLinesTriag(figs,lines,size,backgroundColor);
        else
            return draw2DLines(lines,size,backgroundColor);
}

Figure createSphere(const int n) {
    Figure sphere = createIcosahedron();
    divideInTriangles(sphere,n);
    for(auto &point:sphere.points){
/*        double r = sqrt(pow(point.x,2) + pow(point.y,2) + pow(point.z,2));
        point.x /= r;
        point.y /= r;
        point.z /= r;*/
        point.normalise();
    }
    return sphere;
}

Figure createCone(int n, double h) {
    double r = 1;
    double pi = 2* acos(0.0);
    vector<Vector3D> points;
    vector<Face> faces;
    for(int i = 0; i <= n; i++){
        if(i != n){
            points.push_back(Vector3D::point(cos(2*i*pi/n),sin(2*i*pi/n),0));
        }
        else if( i == n){
            points.push_back(Vector3D::point(0,0,h));
        }
    }
    for(int i = 0; i <= n; i++){
        if(i != n)faces.push_back(Face({i,(i+1)%n,n}));
        else if( i == n){
            Face base;
            for(int j = 1; j <= n ; j++){
                base.point_indices.push_back(n - j);
            }
            faces.push_back(base);
        }
    }
    return Figure(points,faces);
}

Figure createCylinder(int n, double h) {
    double r = 1;
    double pi = 2* acos(0.0);
    vector<Vector3D> points;
    vector<Face> faces;
    // tegenwijzerszin van groot naar klein
    points.reserve(n*2);
    for(int i = 0; i < n; i++){
        points.push_back(Vector3D::point(cos(2*i*pi/n),sin(2*i*pi/n),0));
    }
    for(int i = 0; i < n; i++){
        points.push_back(Vector3D::point(cos(2*i*pi/n),sin(2*i*pi/n),h));
    }
    Face base;
    for(int i = 1; i <= n ; i++){
        base.point_indices.push_back(n - i);
    }
    faces.push_back(base);
    Face roof;
    for(int i = 1; i <= n ; i++){
        roof.point_indices.push_back(2*n - i);
    }
    faces.push_back(roof);
    // connecting lines
    for(int i = 0; i < n; i++){
        faces.push_back(Face({i,(i + 1)%n,n + (i + 1)%n,n + i}));
    }
    return Figure(points,faces);
}

Figure createTorus(double r, double R, int n, int m) {
    Figure torus;
    double pi = 2* acos(0.0);
    vector<Vector3D> points;
    vector<Face> faces;
    for(int i = 0; i < n; i++){
        double u = 2*i*pi/n ;
        for(int j = 0; j < m; j++){
            double v = 2*j*pi/m;
            double x = (R + r*cos(v))*cos(u);
            double y = (R + r*cos(v))*sin(u);
            double z = r*sin(v);
            points.push_back(Vector3D::point(x,y,z));
        }
    }
    for(int i = 0; i < n; i++){
        for(int j = 0; j < m; j++){
            int pointFormula1 = i*m + j ;
            int pointFormula2 = i * m + (j + 1) % m ;
            int pointFormula3 = ((i + 1) % n) * m + (j + 1) % m;
            int pointFormula4 = ((i + 1) % n) * m + j;
            faces.push_back(Face({pointFormula1,pointFormula2,pointFormula3,pointFormula4}));
        }
    }
    torus.faces = faces;
    torus.points = points;
    return torus;
}

Figure createTetrahedron() {
    vector<Vector3D> points = {Vector3D::point(1,-1,-1),Vector3D::point(-1,1,-1),Vector3D::point(1,1,1),Vector3D::point(-1,-1,1)};
    vector<Face> faces = {Face({1,2,3}),Face({2,4,3}),Face({1,4,2}),Face({1,3,4})};
    decrementFaces(faces);
    return Figure(points,faces);
}

Figure createIcosahedron() {
    double pi = 2* acos(0.0);
    vector<Vector3D> points = {Vector3D::point(0,0,sqrt(5)/2)};
    vector<Face> faces = {Face({1,2,3}),Face({1,3,4}),Face({1,4,5}),Face({1,5,6}),Face({1,6,2}),Face({2,7,3}),Face({3,7,8}),Face({3,8,4}),Face({4,8,9}),Face({4,9,5}),Face({5,9,10}),Face({5,10,6}), Face({6,10,11}), Face({6,11,2}), Face({2,11,7}), Face({12,8,7}), Face({12,9,8}), Face({12,10,9}), Face({12,11,10}), Face({12,7,11})};
    decrementFaces(faces);
    for(int i = 2 ; i <= 12; i++){
        if(i > 1 and i <=6)points.push_back(Vector3D::point(cos((i - 2)*2*pi/5),sin((i - 2)*2*pi/5),0.5));
        else if(i > 6 and i < 12)points.push_back(Vector3D::point(cos(pi/5 + (i - 7)*2*pi/5),sin(pi/5 + (i - 7)*2*pi/5),-0.5));
        else if(i == 12)points.push_back(Vector3D::point(0,0,-sqrt(5)/2));
    }
    return Figure(points,faces);
}

void divideInTriangles(Figure &icosahedron,int n) {
    vector<Face> faces = icosahedron.faces;
    vector<Vector3D> points = icosahedron.points;
    vector<Vector3D> tempPoints;
    vector<Face> tempFaces;
    int count = 0;
    for(int i = 0; i < n; i++) {
        for (auto face: faces) {
            int indexA = face.point_indices[0];
            int indexB = face.point_indices[1];
            int indexC = face.point_indices[2];
            Vector3D A = points[indexA];
            Vector3D B = points[indexB];
            Vector3D C = points[indexC];
            Vector3D D = (A + B) / 2;
            Vector3D E = (A + C) / 2;
            Vector3D F = (B + C) / 2;
            tempPoints.push_back(F);
            tempPoints.push_back(E);
            tempPoints.push_back(D);
            tempPoints.push_back(C);
            tempPoints.push_back(B);
            tempPoints.push_back(A);
            int indexF = count;
            int indexE = count + 1;
            int indexD = count + 2;
            indexC = count + 3;
            indexB = count + 4;
            indexA = count + 5;
            tempFaces.push_back(Face({indexA,indexD,indexE}));
            tempFaces.push_back(Face({indexB,indexF,indexD}));
            tempFaces.push_back(Face({indexC,indexE,indexF}));
            tempFaces.push_back(Face({indexE,indexD,indexF}));
            count += 6;
        }
        faces = tempFaces;
        tempFaces = {};
        points = tempPoints;
        tempPoints = {};
        count = 0;
    }
    icosahedron.faces = faces;
    if(!points.empty())
        icosahedron.points = points;
}

Figure createCube() {
    vector<Vector3D> points = {Vector3D::point(1,-1,-1),Vector3D::point(-1,1,-1),Vector3D::point(1,1,1),Vector3D::point(-1,-1,1),Vector3D::point(1,1,-1),Vector3D::point(-1,-1,-1),Vector3D::point(1,-1,1),Vector3D::point(-1,1,1)};
    vector<Face> faces = {Face({1,5,3,7}),Face({5,2,8,3}),Face({2,6,4,8}),Face({6,1,7,4}),Face({7,3,8,4}),Face({1,6,2,5})};
    decrementFaces(faces);
    return Figure(points,faces);
}

Figure createOctahedron() {
    vector<Vector3D> points = {Vector3D::point(1,0,0),Vector3D::point(0,1,0),Vector3D::point(-1,0,0),Vector3D::point(0,-1,0),Vector3D::point(0,0,-1),Vector3D::point(0,0,1)};
    vector<Face> faces = {Face({1,2,6}),Face({2,3,6}),Face({3,4,6}),Face({4,1,6}),Face({2,1,5}),Face({3,2,5}),Face({4,3,5}),Face({1,4,5})};
    decrementFaces(faces);
    return Figure(points,faces);
}

Figure createDodecahedron() {
    double pi = 2* acos(0.0);
    vector<Vector3D> temppoints = {Vector3D::point(0,0,sqrt(5)/2)};
    vector<Vector3D> realPoints;
    vector<Face> faces = {Face({1,2,3}),Face({1,3,4}),Face({1,4,5}),Face({1,5,6}),Face({1,6,2}),Face({2,7,3}),Face({3,7,8}),Face({3,8,4}),Face({4,8,9}),Face({4,9,5}),Face({5,9,10}),Face({5,10,6}), Face({6,10,11}), Face({6,11,2}), Face({2,11,7}), Face({12,8,7}), Face({12,9,8}), Face({12,10,9}), Face({12,11,10}), Face({12,7,11})};
    vector<Face> realFaces = {Face({1,2,3,4,5}),Face({1,6,7,8,2}),Face({2,8,9,10,3}),Face({3,10,11,12,4}),Face({4,12,13,14,5}),Face({5,14,15,6,1}),Face({20,19,18,17,16}),Face({20,15,14,13,19}),Face({19,13,12,11,18}),Face({18,11,10,9,17}),Face({17,9,8,7,16}),Face({16,7,6,15,20})};
    decrementFaces(faces);
    decrementFaces(realFaces);
    for(int i = 2 ; i <= 12; i++){
        if(i > 1 and i <=6)temppoints.push_back(Vector3D::point(cos((i - 2)*2*pi/5),sin((i - 2)*2*pi/5),0.5));
        else if(i > 6 and i < 12)temppoints.push_back(Vector3D::point(cos(pi/5 + (i - 7)*2*pi/5),sin(pi/5 + (i - 7)*2*pi/5),-0.5));
        else if(i == 12)temppoints.push_back(Vector3D::point(0,0,-sqrt(5)/2));
    }
    for(auto face:faces){
        double midX = (temppoints[face.point_indices[0]].x + temppoints[face.point_indices[1]].x + temppoints[face.point_indices[2]].x)/3;
        double midY = (temppoints[face.point_indices[0]].y + temppoints[face.point_indices[1]].y + temppoints[face.point_indices[2]].y)/3;
        double midZ = (temppoints[face.point_indices[0]].z + temppoints[face.point_indices[1]].z + temppoints[face.point_indices[2]].z)/3;
        realPoints.push_back(Vector3D::point(midX,midY,midZ));
    }
    return Figure(realPoints,realFaces);
}

void decrementFaces(vector<Face> &faces) {
    for(auto &face:faces){
        for(auto &point:face.point_indices){
            point -= 1;
        }
    }
}
struct Resource {
    Vector3D currentPos;
    Vector3D HVector;
    Vector3D LVector;
    Vector3D UVector;
    Resource(const Vector3D& currentPos1, const Vector3D& HVector1,const Vector3D& LVector1,const Vector3D& UVector1 ){
        HVector = HVector1;
        currentPos = currentPos1;
        UVector = UVector1;
        LVector = LVector1;
    }
};
Figure draw3DLSystem(const ini::Configuration &conf,const string &figureName) {
    LParser::LSystem3D l_system;
    ifstream input_stream(conf[figureName]["inputfile"].as_string_or_die());
    input_stream >> l_system;
    input_stream.close();
    stack<Resource> bracketContainer;
    double angle = l_system.get_angle();
    vector<char> legalSymbols = {'+','-','(',')','^','&','\\','/','|'};
    legalSymbols.insert(legalSymbols.begin(),l_system.get_alphabet().begin(),l_system.get_alphabet().end());
    string initiator = l_system.get_initiator();
    string drawString = replaceWithRules3D(initiator,l_system,int(l_system.get_nr_iterations()));
    Vector3D huidigPositie = Vector3D::point(0, 0, 0);
    Vector3D HVector = Vector3D::vector(1, 0, 0);
    Vector3D LVector = Vector3D::vector(0, 1, 0);
    Vector3D UVector = Vector3D::vector(0, 0, 1);
    Figure figure;
    for (auto character: drawString) {
        double radian = angle * (2 * acos(0.0) / 180);
        if (contains(legalSymbols, character)) {
            Vector3D oldHVector = HVector;
            Vector3D oldLVector = LVector;
            Vector3D oldUVector = UVector;
            if (character == '-') {
                HVector = oldHVector * cos(-radian) + oldLVector * sin(-radian);
                LVector = -oldHVector * sin(-radian) + oldLVector * cos(-radian);
            } else if (character == '+') {
                HVector = oldHVector * cos(radian) + oldLVector * sin(radian);
                LVector = -oldHVector * sin(radian) + oldLVector * cos(radian);
            } else if (character == '^') {
                HVector = oldHVector * cos(radian) + oldUVector * sin(radian);
                UVector = -oldHVector * sin(radian) + oldUVector * cos(radian);
            } else if (character == '&') {
                HVector = oldHVector * cos(-radian) + oldUVector * sin(-radian);
                UVector = -oldHVector * sin(-radian) + oldUVector * cos(-radian);
            } else if (character == '\\') {
                LVector = oldLVector * cos(radian) - oldUVector * sin(radian);
                UVector = oldLVector * sin(radian) + oldUVector * cos(radian);
            } else if (character == '/') {
                LVector = oldLVector * cos(-radian) - oldUVector * sin(-radian);
                UVector = oldLVector * sin(-radian) + oldUVector * cos(-radian);
            } else if (character == '|') {
                HVector = -oldHVector;
                LVector = -oldLVector;
            } else if (character == '(')
                bracketContainer.emplace(huidigPositie,HVector,LVector,UVector);
            else if (character == ')') {
                Resource resource = bracketContainer.top();
                huidigPositie = resource.currentPos;
                HVector = resource.HVector;
                LVector = resource.LVector;
                UVector = resource.UVector;
                bracketContainer.pop();
            } else {
                if (l_system.draw(character) == 1) {
                    figure.points.push_back(huidigPositie);
                    huidigPositie += HVector;
                    Vector3D point2 = Vector3D::point(huidigPositie.x, huidigPositie.y, huidigPositie.z);
                    figure.points.push_back(point2);
                    figure.faces.push_back(Face({int(figure.points.size()) - 2,int(figure.points.size()) - 1}));
                } else if (l_system.draw(character) == 0) {
                    huidigPositie += HVector;
                }
            }
        }
    }
    return figure;
}

void draw_zbuf_line(ZBuffer &zbuffer, img::EasyImage &image, const Point2D &point1, const Point2D &point2,
                    const Color &color) 
                    {
    int x0 = lround(point1.x);
    int y0 = lround(point1.y);
    double z0 = point1.zbuf;
    int x1 = lround(point2.x);
    int y1 = lround(point2.y);
    double z1 = point2.zbuf;
    unsigned int width = image.get_width();
    unsigned int height = image.get_height();
    img::Color newColor = img::Color(floor(color.red * 255),floor(color.green * 255),floor(color.blue * 255));
    if (x0 >= width || y0 >= height || x1 >= width || y1 >= height) {
        std::stringstream ss;
        ss << "Drawing line from (" << x0 << "," << y0 << ") to (" << x1 << "," << y1 << ") in image of width "
           << width << " and height " << height;
        throw std::runtime_error(ss.str());
    }
    if (x0 == x1)
    {
        //special case for x0 == x1
        unsigned int a = max(y0,y1) - min(y0,y1) + 1;
        unsigned int current = a;
        for (unsigned int i = std::min(y0, y1); i <= std::max(y0, y1); i++)
        {
            // zelfe probleem als daaronder
            double p = (double)current/a;
            double zP;
            zP = p/z0 + (1 - p)/z1;
            if(zbuffer.get(x0,int(i)) > zP){
                image(x0, i) = newColor;
                zbuffer.set(x0,int(i),zP);
            }
            current--;
        }
    }
    else if (y0 == y1)
    {
        //special case for y0 == y1
        unsigned int a = max(x0,x1) - min(x0,x1) + 1;
        unsigned int current = a;
        for (unsigned int i = min(x0, x1); i <= std::max(x0, x1); i++)
        {
            double p = (double)current/a;
            double zP;
            zP = p/z0 + (1 - p)/z1;
            if(zbuffer.get(int(i), y0) > zP ){
                image(i, y0) = newColor;
                zbuffer.set(int(i), y0,zP);
            }
            current--;
        }
    }
    else
    {
        if (x0 > x1)
        {
            //flip points if x1>x0: we want x0 to have the lowest value
            std::swap(x0, x1);
            std::swap(y0, y1);
            swap(z0,z1);
        }

        double m = ((double) y1 - (double) y0) / ((double) x1 - (double) x0);
        if (-1.0 <= m && m <= 1.0)
        {
            unsigned int a = max(x0,x1) - min(x0,x1) + 1;
            unsigned int current = a;
            for (unsigned int i = 0; i <= (x1 - x0); i++)
            {
                double p = (double)current / a;
                double zP;
                zP = p/z0 + (1 - p)/z1;
                int curX = x0 + int(i);
                int curY = (int) round(y0 + m * i);
                if(zbuffer.get(curX,curY) > zP){
                    image(curX, curY) = newColor;
                    zbuffer.set(curX,curY,zP);
                }
                current--;
            }
        }
        else if (m > 1.0)
        {
            unsigned int a = max(y0,y1) - min(y0,y1) + 1;
            unsigned int current = a;
            for (unsigned int i = 0; i <= (y1 - y0); i++)
            {
                double p = (double)current / a;
                double zP;
                zP = p/z0 + (1 - p)/z1;
                int curX = (int) round(x0 + (i / m));
                int curY = y0 + int(i);
                if(zbuffer.get(curX,curY) > zP){
                    image(curX, curY) = newColor;
                    zbuffer.set(curX,curY,zP);
                }
                current--;
            }
        }
        else if (m < -1.0)
        {
            unsigned int a = max(y0,y1) - min(y0,y1) + 1;
            unsigned int current = a;
            for (unsigned int i = 0; i <= (y0 - y1); i++)
            {
                double p = (double)current / a;
                double zP;
                zP = p/z0 + (1 - p)/z1;
                int curX = (int) round(x0 - (i / m));
                int curY = y0 - int(i);
                if(zbuffer.get(curX,curY) > zP ){
                    image(curX, curY) = newColor;
                    zbuffer.set(curX,curY,zP);
                }
                current--;
            }
        }
    }
}

Point2D doProjection2(const Vector3D &point, double d) {
    return Point2D(d * point.x / -point.z, d * point.y / -point.z,point.z);
}

Lines2DArr doProjection2(const Figures3D &figures) {
    Lines2DArr arr = {};
    for(auto fig:figures){
        for(auto face:fig.faces){
            if(int(face.point_indices.size()) == 2){
                Point2D point1 = doProjection2(fig.points[face.point_indices[0]],1);
                Point2D point2 = doProjection2(fig.points[face.point_indices[1]],1);
                arr.emplace_back(point1,point2,fig.color);
            }
            else if(int(face.point_indices.size()) > 2){
                for(int i = 0; i < int(face.point_indices.size()); i++){
                    Point2D point1 = doProjection2(fig.points[face.point_indices[i]],1);
                    Point2D point2 = doProjection2(fig.points[face.point_indices[(i+1)%int(face.point_indices.size())]],1);
                    arr.emplace_back(point1,point2,fig.color);
                }
            }

        }
    }
    return arr;
}

vector<Face> triangulate(const Face &face) {
    vector<Face> faces;
    int n = int(face.point_indices.size());
    for(int i = 0; i < n - 2; i++){
        Face newFace;
        for(int j = 0; j < 3; j++){
            if( j == 0)newFace.point_indices.push_back(face.point_indices[0]);
            else newFace.point_indices.push_back(face.point_indices[j+i]);
        }
        faces.push_back(newFace);
    }
    return faces;
}


vector<double> getMinMaxValues(const vector<Lines2D>& lines) {
    Lines2D line1 = lines[0];
    double xMin = line1.p1.x;
    double xMax = line1.p1.x;
    double yMin = line1.p1.y;
    double yMax = line1.p1.y;
    for(auto line:lines){
        const Point2D point = line.p2;
        const Point2D point2 = line.p1;

        xMin = min(xMin,point.x);
        xMax = max(xMax,point.x);
        xMin = min(xMin,point2.x);
        xMax = max(xMax,point2.x);

        yMin = min(yMin,point.y);
        yMax = max(yMax,point.y);
        yMin = min(yMin,point2.y);
        yMax = max(yMax,point2.y);
    }
    vector<double> extremas = {xMin,xMax,yMin,yMax};
    return extremas;
}

double getImageX(vector<double> &extremas, int size) {
    double xMin = extremas[0];
    double xMax = extremas[1];
    double yMin = extremas[2];
    double yMax = extremas[3];
    double xRange = xMax - xMin ;
    double yRange = yMax - yMin;
    double imageX = size * xRange / max(xRange,yRange);
    return imageX;
}

double getImageY(vector<double> &extremas, int size) {
    double xMin = extremas[0];
    double xMax = extremas[1];
    double yMin = extremas[2];
    double yMax = extremas[3];
    double xRange = xMax - xMin ;
    double yRange = yMax - yMin;
    double imageY = size * yRange / max(xRange,yRange);
    return imageY;
}

double getdx(vector<double> &extremas, int size) {
    double xMin = extremas[0];
    double xMax = extremas[1];
    double yMin = extremas[2];
    double yMax = extremas[3];
    double xRange = xMax - xMin;
    double yRange = yMax - yMin;
    double imageX = size * xRange / max(xRange,yRange);
    double d = 0.95 * imageX / xRange;
    double DCx = d * (xMin + xMax) / 2;
    double DCy = d * (yMin + yMax) / 2;
    double dx = imageX / 2 - DCx;
    return dx;
}

double getdy(vector<double> &extremas, int size) {
    double xMin = extremas[0];
    double xMax = extremas[1];
    double yMin = extremas[2];
    double yMax = extremas[3];
    double xRange = xMax - xMin;
    double yRange = yMax - yMin;
    double imageY = size * yRange / max(xRange,yRange);
    double imageX = size * xRange / max(xRange,yRange);
    double d = 0.95 * imageX / xRange;
    double DCx = d * (xMin + xMax) / 2;
    double DCy = d * (yMin + yMax) / 2;
    double dx = imageX / 2 - DCx;
    double dy = imageY / 2 - DCy;
    return dy;
}

void draw(ZBuffer &zBuffer, img::EasyImage& image, const vector<Lines2D> &lines, double d, double dx, double dy, double imageX,
          double imageY, const ini::DoubleTuple &backgroundColor) {
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();
    Lines2D line1 = lines[0];
    //ZBuffer zBuffer(floor(imageX),floor(imageY));
    bool usesZbuffer = false;
    if(line1.p1.zbuf != posInf)
        usesZbuffer = true;
    for(int y = 0; y < floor(imageY); y++){
        for(int x = 0; x < floor(imageX); x++){
            image(x,y).red = floor(backgroundColor[0] * 255);
            image(x,y).green = floor(backgroundColor[1] * 255);
            image(x,y).blue = floor(backgroundColor[2] * 255);
        }
    }
    for(auto line:lines){
        line.p1.x *= d;
        line.p1.y *= d;
        line.p2.x *= d;
        line.p2.y *= d;
        line.p1.x += dx;
        line.p1.y += dy;
        line.p2.x += dx;
        line.p2.y += dy;
        if(usesZbuffer)
            draw_zbuf_line(zBuffer,image,line.p1,line.p2,line.color);
        else
            image.draw_line(floor(line.p1.x),floor(line.p1.y),floor(line.p2.x),floor(line.p2.y),img::Color(floor(line.color.red*255),floor(line.color.green*255),floor(line.color.blue*255)));
    }
}

void triangulate(Figure& figure) {
    vector<Face> faces;
    for(auto& face:figure.faces){
        face.triangulate(faces);
    }
    figure.faces = faces;
}

void
draw_zbuf_triag(ZBuffer &zbuffer, img::EasyImage &image, Figure &figure, double d, double dx, double dy, Color color,const ini::DoubleTuple& backgroundColor) {
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();
    vector<Lines2D> lines;
    for(auto& face:figure.faces){
        const Vector3D A = figure.points[face.point_indices[0]];
        const Vector3D B = figure.points[face.point_indices[1]];
        const Vector3D C = figure.points[face.point_indices[2]];
        draw_zbuf_triag(image,zbuffer,A,B,C,d,dx,dy,backgroundColor,color);
    }
}
/**/
img::EasyImage draw2DLinesTriag(Figures3D &figures,vector<Lines2D>& lines, int size,
                                const ini::DoubleTuple &backgroundColor) {

    if(int(lines.size()) == 0 or size == 0)img::EasyImage image(100,100) ;
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();
    Lines2D line1 = lines[0];
    double xMin = line1.p1.x;
    double xMax = line1.p1.x;
    double yMin = line1.p1.y;
    double yMax = line1.p1.y;
    double xRange = 0;
    double yRange = 0;
    double imageX = 0;
    double imageY = 0;
    double DCx = 0;
    double DCy = 0;
    double dx = 0;
    double dy = 0;
    double d = 0;
    vector<double> extremas = getMinMaxValues(lines);
    xMin = extremas[0];
    xMax = extremas[1];
    yMin = extremas[2];
    yMax = extremas[3];
    xRange = xMax - xMin;
    yRange = yMax - yMin;
    imageX = getImageX(extremas,size);
    imageY = getImageY(extremas,size);
    ZBuffer zBuffer(floor(imageX),floor(imageY));
    d = 0.95 * imageX / xRange;
    dx = getdx(extremas,size);
    dy = getdy(extremas,size);
    img::EasyImage image(floor(imageX),floor(imageY));
    for(auto &figure:figures){
        draw_zbuf_triag(zBuffer,image,figure,d,dx,dy,figure.color,backgroundColor);
    }
    return image;
}

void draw_zbuf_triag(img::EasyImage &image, ZBuffer &zbuffer, const Vector3D &A, const Vector3D &B, const Vector3D &C,
                     double d, double dx, double dy, const ini::DoubleTuple &backgroundColor, Color color) {
    
    Point2D a = doProjection(A,d,dx,dy);
    Point2D b = doProjection(B,d,dx,dy);
    Point2D c = doProjection(C,d,dx,dy);
    Vector3D zwaartePunt = Vector3D::vector((a.x + b.x + c.x) / 3,(a.y + b.y + c.y) / 3,(1/(3*a.zbuf)) + (1/(3*b.zbuf)) + (1/(3*c.zbuf)) );

    Vector3D u = B - A;
    Vector3D v = C - A;
    Vector3D normaalVector = Vector3D::cross(u,v);
    normaalVector.normalise();

    double w1 = normaalVector.x;
    double w2 = normaalVector.y;
    double w3 = normaalVector.z;
    double k = w1 * A.x + w2 * A.y + w3 * A.z;

    double dzdx = w1 / (-d * k);
    double dzdy = w2 / (-d * k);

    long yMin = lround(min(min(a.y,b.y),c.y) + 0.5);
    long yMax = lround(max(max(a.y,b.y),c.y) - 0.5);

    for(long y = yMin; y <= yMax; y++){
        long xL;
        long xR;
        calculatexLAndxR(y,a,b,c,xL,xR);
        for(long x = xL; x <= xR; x++){
            double zP = (1.000001 * zwaartePunt.z) + (x - zwaartePunt.x) * dzdx + (y - zwaartePunt.y) * dzdy;
            if(zbuffer.get(x,y) > zP){
                image(x,y) = img::Color(floor(color.red * 255),floor(color.green * 255),floor(color.blue * 255));
                zbuffer.set(x,y,zP);
            }
        }
    }
}

Point2D doProjection(const Vector3D &point, double d, double dx, double dy) {
    return Point2D(d * point.x / -point.z + dx, d * point.y / -point.z + dy,point.z);
}

void calculatexLAndxR(long y, Point2D &a, Point2D &b, Point2D &c, long &xL, long &xR) {
    double posInf = std::numeric_limits<double>::infinity();
    double negInf = -std::numeric_limits<double>::infinity();

    double xLAB = posInf;
    double xLAC = posInf;
    double xLBC = posInf;

    double xRAB = negInf;
    double xRAC = negInf;
    double xRBC = negInf;

    if(a.y != b.y && double(y - a.y)*(y - b.y) <= 0){
        double xI = b.x + double(a.x - b.x) * double(y - b.y) / double(a.y - b.y);
        if(xI < xLAB){
            xLAB = xI;
        }
        if(xI > xRAB){
            xRAB = xI;
        }
    }
    if(a.y != c.y && (y - a.y)*(y - c.y) <= 0){
        double xI = c.x + double(a.x - c.x) * double(y - c.y) / double(a.y - c.y);
        if(xI < xLAC){
            xLAC = xI;
        }
        if(xI > xRAC){
            xRAC = xI;
        }
    }
    if(b.y != c.y && (y - b.y)*(y - c.y) <= 0){
        double xI = c.x + double(b.x - c.x) * double(y - c.y) / double(b.y - c.y);
        if(xI < xLBC){
            xLBC = xI;
        }
        if(xI > xRBC){
            xRBC = xI;
        }
    }
    xL = lround(min(min(xLAB,xLAC),xLBC) + 0.5);
    xR = lround(max(max(xRAB,xRAC),xRBC) - 0.5);
}




