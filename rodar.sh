#!/bin/bash

gcc -c cg2d.c
gcc -c cg3d.c
gcc 3d.c -o main cg2d.o cg3d.o bibaux.o -lm -lX11
rm cg2d.o cg3d.o
./main
rm main