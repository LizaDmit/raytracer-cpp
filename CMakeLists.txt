#Prevents using an older CMake version that might not support features used in this file
cmake_minimum_required(VERSION 3.16)

#Defines the project name and the language used
project(raytracer LANGUAGES CXX)

#Tells CMake to use C++17 standard
set(CMAKE_CXX_STANDARD 17)

#Enforces C++17 strictly.Prevents fallback to an older C++ version
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#Creates an executable named raytracer. Compiles src/main.cpp
add_executable(raytracer src/main.cpp)