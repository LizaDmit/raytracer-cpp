#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
using namespace std;

using color = vec3;

void write_color(ostream& out, const color& pixel_color) {
    
    auto r = pixel_color.x();       // Type double but for convention auto is used
    auto g = pixel_color.y();
    auto b = pixel_color.z();

    int rbyte = int(255.999 * r);   // From [0,1] format to [0,256] format
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);

    out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';   // Output the pixels as required for PPM file
}

#endif