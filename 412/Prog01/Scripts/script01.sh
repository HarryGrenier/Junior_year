#!/bin/bash

gcc ../Source/prog01v1.c -o prog01v1.exe
chmod 704 prog01v1.exe

for arg in "$@"; do
    ./prog01v1.exe "$arg"
done
