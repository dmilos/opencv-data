#!/bin/bash

# Make all examples.
# No Scons, no CMake, no Make or any other make tool
# One line one example
# Designed for easy copy/paste

# TODO REFLECTION_USE_PP2FILE

reset

pwd

g++ -fexceptions -std=c++11 -g printer.cpp   -o  printer