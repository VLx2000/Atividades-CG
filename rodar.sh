#!/bin/bash

cd src
gcc -c cg2d.c
gcc -c cg3d.c
gcc ../3d.c -o main cg2d.o cg3d.o bibaux.o -lm -lX11
./main
rm main cg2d.o cg3d.o
cd ..