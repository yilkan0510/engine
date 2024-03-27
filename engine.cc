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
    double angle = l_system.get_angle();
    std::set<char> alphabet = l_system.get_alphabet();


    double x0 = 0;
    double y0 = 0;
    double angleRad = angle * M_PI / 180;

    std::stack<std::tuple<double, double, double>> stateStack;



    for (char c: input){
        if (alphabet.find(c) != alphabet.end()){
            if (l_system.draw(c)){
                // Calculate new endpoint coordinates based on the current position, angle, and rules
                double new_x1 = x0 + cos(angleRad);
                double new_y1 = y0 + sin(angleRad);

                fig.points.push_back(Vector3D::point(x0, y0, 0));
                fig.points.push_back(Vector3D::point(new_x1, new_y1, 0));
                fig.faces.push_back(Face({(int)fig.points.size() - 2, (int)fig.points.size() - 1}));

                x0 = new_x1;
                y0 = new_y1;
            }
        } else if (c == '+') {
            angleRad += angle * M_PI / 180;
        } else if (c == '-') {
            angleRad -= angle * M_PI / 180;
        } else if (c == '(') {
            stateStack.push(std::make_tuple(x0, y0, angleRad));
        } else if (c == ')') {
            if (!stateStack.empty()) {
                std::tie(x0, y0, angleRad) = stateStack.top();
                stateStack.pop();
            }
        }
    }
    return fig;
}


img::EasyImage generate_image(const ini::Configuration &configuration) {
    int size = configuration["General"]["size"].as_int_or_die();
    auto background_color = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();
    int nrFigures = configuration["General"]["nrFigures"].as_int_or_die();
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


        }

        else if ("Tetrahedron" == type) {
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

    img::EasyImage image = drawlines2D(projectedLines, size, background_color );

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