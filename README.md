```md
# Ray Tracer in One Weekend (C++)

A C++ implementation of the "Ray Tracing in One Weekend" book/tutorial.
Goal: build a simple ray tracer from scratch, learn rendering fundamentals,
and practice clean C++ design.

## Build

This project uses **CMake**, a cross-platform build system for C++.

The following commands configure the build, compile the program, and run it
to generate an image in PPM format.

```bash
cmake -S . -B build
cmake --build build
./build/raytracer > images/out.ppm
```