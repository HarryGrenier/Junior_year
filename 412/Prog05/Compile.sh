#!/bin/bash

g++ Programs/Version1/prog05v1.cpp --std=c++20 -o Compiled/prog05v1.exe
g++ Programs/Version2/prog05v2.cpp --std=c++20 -o Compiled/prog05v2.exe
g++ Programs/Version3/prog05v3.cpp --std=c++20 -o Compiled/prog05v3.exe

chmod +x Compiled/*.exe