#include "easy_image.h"
#include "ini_configuration.h"
#include "l_parser.h"
#include "DrieDfiguur.h"
#include "Transformaties.h"
#include "vector3d.h"
#include "drawlines.h"
#include "Eye_point_transformaties.h"
#include "color-point-line.h"
#include "Perspectiefprojectie.h"
#include "ZBuffer.h"
#include <algorithm>

#define _USE_MATH_DEFINES

#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <cmath>
#include <iostream>
#include <stack>
#include "FigureMaker3D.h"
std::string ProcessString(const LParser::LSystem3D &l_system, const std::string &input) {
    std::string new_result = "";
    for (char c : input) {
        if (l_system.get_alphabet().find(c) != l_system.get_alphabet().end()) {
            new_result += l_system.get_replacement(c);
        } else {
            new_result += c;
        }
    }
    return new_result;
}

std::string BuildString(LParser::LSystem3D &l_system, unsigned int iterations) {
    std::string result = l_system.get_initiator();
    for (unsigned int i = 0; i < iterations; i++) {
        result = ProcessString(l_system, result);
    }
    return result;
}
Figure BuildFigureFromString(const std::string &input, LParser::LSystem3D &l_system) {
    Figure fig;
    //angle in radian
    double angle = l_system.get_angle() * M_PI / 180;
    std::set<char> alphabet = l_system.get_alphabet();

    Vector3D H = Vector3D::point(1, 0, 0);
    Vector3D L = Vector3D::point(0, 1, 0);
    Vector3D U = Vector3D::point(0, 0, 1);

    std::stack<std::tuple<Vector3D, Vector3D, Vector3D, Vector3D>> stateStack;
    Vector3D currentPoint = Vector3D::point(0, 0, 0);

    for (char c: input){

        if (alphabet.find(c) != alphabet.end()){
            if (l_system.draw(c)){
                Vector3D newPoint = currentPoint + H;
                fig.points.push_back(currentPoint);
                fig.points.push_back(newPoint);
                fig.faces.push_back(Face({(int)fig.points.size() - 2, (int)fig.points.size() - 1}));
                currentPoint = newPoint;
            }
        } else if (c == '+') {
            Vector3D Hnew = H * cos(angle) + L * sin(angle);
            L = H * -sin(angle) + L * cos(angle);
            H = Hnew;
        } else if (c == '-') {
            Vector3D Hnew = H * cos(-angle) + L * sin(-angle);
            L = H * -sin(-angle) + L * cos(-angle);
            H = Hnew;
        } else if (c == '^') {
            Vector3D Hnew = H * cos(angle) + U * sin(angle);
            U = H * -sin(angle) + U * cos(angle);
            H = Hnew;
        } else if (c == '&') {
            Vector3D Hnew = H * cos(-angle) + U * sin(-angle);
            U = H * -sin(-angle) + U * cos(-angle);
            H = Hnew;
        } else if (c == '\\') {
            Vector3D Lnew = L * cos(angle) - U * sin(angle);
            U = L * sin(angle) + U * cos(angle);
            L = Lnew;
        } else if (c == '/') {
            Vector3D Lnew = L * cos(-angle) - U * sin(-angle);
            U = L * sin(-angle) + U * cos(-angle);
            L = Lnew;
        } else if (c == '|') {
            H = H * -1;
            L = L * -1;
        } else if (c == '(') {
            stateStack.push(std::make_tuple(currentPoint, H, L, U));
        } else if (c == ')') {
            if (!stateStack.empty()) {
                auto state = stateStack.top();
                stateStack.pop();
                currentPoint = std::get<0>(state);
                H = std::get<1>(state);
                L = std::get<2>(state);
                U = std::get<3>(state);
            }
        }
    }
    return fig;
}

img::EasyImage generate_image(const ini::Configuration &configuration) {
    int size = configuration["General"]["size"].as_int_or_die();
    auto background_color = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
    int nrFigures = configuration["General"]["nrFigures"].as_int_or_die();

    bool Zbuf = false;
    std::string GeneralType = configuration["General"]["type"].as_string_or_die();
    if (GeneralType == "ZBufferedWireframe") {
        Zbuf = true;
    } else {
        Zbuf = false;
    }

    Figures3D figures;

    for (int i = 0; i < nrFigures; i++) {
        std::string figureKey = "Figure" + std::to_string(i);
        Figure fig;

        std::string type = configuration[figureKey]["type"].as_string_or_die();
        double scale = configuration[figureKey]["scale"].as_double_or_die();
        auto rotateX = configuration[figureKey]["rotateX"].as_double_or_die();
        auto rotateY = configuration[figureKey]["rotateY"].as_double_or_die();
        auto rotateZ = configuration[figureKey]["rotateZ"].as_double_or_die();
        auto center = configuration[figureKey]["center"].as_double_tuple_or_die();
        auto color = configuration[figureKey]["color"].as_double_tuple_or_die();

        FigureMaker3D figureMaker3D;
        if ("3DLSystem" == type) {
            std::string inputfile = configuration[figureKey]["inputfile"].as_string_or_die();
            LParser::LSystem3D l_system;
            std::ifstream input_stream(inputfile);
            input_stream >> l_system;
            input_stream.close();

            std::string result = BuildString(l_system, l_system.get_nr_iterations());

            fig = BuildFigureFromString(result, l_system);
        } else if ("Tetrahedron" == type) {
            fig = figureMaker3D.createTetrahedron();
        } else if ("Octahedron" == type) {
            fig = figureMaker3D.createOctahedron();
        } else if ("Dodecahedron" == type) {
            fig = figureMaker3D.createDodecahedron();
        } else if ("Cube" == type) {
            fig = figureMaker3D.createCube();
        } else if ("Icosahedron" == type) {
            fig = figureMaker3D.createIcosahedron();
        } else if ("Sphere" == type) {
            auto divideAmount = configuration[figureKey]["n"].as_int_or_die();
            fig = figureMaker3D.createSphere(divideAmount);
        } else if ("Cone" == type) {
            auto n = configuration[figureKey]["n"].as_int_or_die();
            auto h = configuration[figureKey]["height"].as_double_or_die();
            fig = figureMaker3D.createCone(n, h);
        } else if ("Cylinder" == type) {
            auto n = configuration[figureKey]["n"].as_int_or_die();
            auto h = configuration[figureKey]["height"].as_double_or_die();
            fig = figureMaker3D.createCylinder(n, h);
        } else if ("Torus" == type) {
            auto r = configuration[figureKey]["r"].as_double_or_die();
            auto R = configuration[figureKey]["R"].as_double_or_die();
            auto n = configuration[figureKey]["n"].as_int_or_die();
            auto m = configuration[figureKey]["m"].as_int_or_die();
            fig = figureMaker3D.createTorus(r, R, n, m);
        } else if ("LineDrawing" == type) {
            int nrPoints = configuration[figureKey]["nrPoints"].as_int_or_die();
            for (int j = 0; j < nrPoints; j++) {
                auto point = configuration[figureKey]["point" + std::to_string(j)].as_double_tuple_or_die();
                fig.points.push_back(Vector3D::point(point[0], point[1], point[2]));
            }
            int nrLines = configuration[figureKey]["nrLines"].as_int_or_die();
            for (int j = 0; j < nrLines; j++) {
                auto line = configuration[figureKey]["line" + std::to_string(j)].as_int_tuple_or_die();
                Face face;
                face.point_indexes.push_back(line[0]);
                face.point_indexes.push_back(line[1]);
                fig.faces.push_back(face);
            }
        }
        fig.color = color;

        Transformaties trans;
        Matrix scaleMatrix = trans.scaleFigure(scale);
        Matrix rotateXMatrix = trans.rotateX(rotateX);
        Matrix rotateYMatrix = trans.rotateY(rotateY);
        Matrix rotateZMatrix = trans.rotateZ(rotateZ);
        Vector3D centerVector = Vector3D::point(center[0], center[1], center[2]);
        Matrix translateMatrix = trans.translate(centerVector);
        Matrix transformationMatrix = scaleMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * translateMatrix;
        trans.applyTransformation(fig, transformationMatrix);

        Eye_point_transformaties eyeTrans;
        auto eye = configuration["General"]["eye"].as_double_tuple_or_die();
        Vector3D eyePoint = Vector3D::point(eye[0], eye[1], eye[2]);
        Matrix eyePointMatrix = eyeTrans.eyePointTrans(eyePoint);
        eyeTrans.applyTranformation(fig, eyePointMatrix);

        figures.push_back(fig);
    }

    Perspectiefprojectie perspProj;

    Lines2D projectedLines = perspProj.doProjectionfig(figures, 1.0);

    img::EasyImage image;
    // Als ZBuffering niet vereist is, teken dan zoals normaal
    image = drawlines2D(projectedLines, size, background_color, Zbuf);
    return image;
}

int main(int argc, char const *argv[]) {
    int retVal = 0;
    try {
        std::vector<std::string> args = std::vector<std::string>(argv + 1, argv + argc);
        if (args.empty()) {
            std::ifstream fileIn("filelist");
            std::string filelistName;
            while (std::getline(fileIn, filelistName)) {
                args.push_back(filelistName);
            }
        }
        for (std::string fileName: args) {
            ini::Configuration conf;
            try {
                std::ifstream fin(fileName);
                if (fin.peek() == std::istream::traits_type::eof()) {
                    std::cout << "Ini file appears empty. Does '" <<
                              fileName << "' exist?" << std::endl;
                    continue;
                }
                fin >> conf;
                fin.close();
            }
            catch (ini::ParseException &ex) {
                std::cerr << "Error parsing file: " << fileName << ": " << ex.what() << std::endl;
                retVal = 1;
                continue;
            }

            img::EasyImage image = generate_image(conf);
            if (image.get_height() > 0 && image.get_width() > 0) {
                std::string::size_type pos = fileName.rfind('.');
                if (pos == std::string::npos) {
                    //filename does not contain a '.' --> append a '.bmp' suffix
                    fileName += ".bmp";
                } else {
                    fileName = fileName.substr(0, pos) + ".bmp";
                }
                try {
                    std::ofstream f_out(fileName.c_str(), std::ios::trunc | std::ios::out | std::ios::binary);
                    f_out << image;

                }
                catch (std::exception &ex) {
                    std::cerr << "Failed to write image to file: " << ex.what() << std::endl;
                    retVal = 1;
                }
            } else {
                std::cout << "Could not generate image for " << fileName << std::endl;
            }
        }
    }
    catch (const std::bad_alloc &exception) {
        //When you run out of memory this exception is thrown. When this happens the return value of the program MUST be '100'.
        //Basically this return value tells our automated test scripts to run your engine on a pc with more memory.
        //(Unless of course you are already consuming the maximum allowed amount of memory)
        //If your engine does NOT adhere to this requirement you risk losing points because then our scripts will
        //mark the test as failed while in reality it just needed a bit more memory
        std::cerr << "Error: insufficient memory" << std::endl;
        retVal = 100;
    }
    return retVal;
}