#!/bin/bash
gcc -c cg2d.c 
gcc atv3cg.c -o atv3cg cg2d.o bibaux.o -lm -lX11 && sudo ./atv3cg