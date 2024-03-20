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

img::EasyImage generate_image(const ini::Configuration &configuration) {
    // Parse general settings
    int size = configuration["General"]["size"].as_int_or_die();
    auto background_color = configuration["General"]["backgroundcolor"].as_double_tuple_or_die();

    int nrFigures = configuration["General"]["nrFigures"].as_int_or_die();
    Figures3D figures;

    for (int i = 0; i < nrFigures; i++) {
        std::string figureKey = "Figure" + std::to_string(i);
        Figure fig;
        double scale = configuration[figureKey]["scale"].as_double_or_die();
        auto rotateX = configuration[figureKey]["rotateX"].as_double_or_die();
        auto rotateY = configuration[figureKey]["rotateY"].as_double_or_die();
        auto rotateZ = configuration[figureKey]["rotateZ"].as_double_or_die();
        auto center = configuration[figureKey]["center"].as_double_tuple_or_die();
        auto color = configuration[figureKey]["color"].as_double_tuple_or_die();
        fig.color = color;

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



// TODO : when i have multiple figures  something goes wrong with the color of the figures, they all end up with the same color but i know why. its because i use the color when im out of the loop, so only the last color gets applied. what should i do to fix this issue?
// TODO : grote van mijn images

