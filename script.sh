#!/bin/bash
gcc -c cg2d.c
gcc atv2cg.c -o atv2cg cg2d.o bibaux.o -lm -lX11 && sudo ./atv2cg

