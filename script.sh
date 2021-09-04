#!/bin/bash

gcc whereismymouse.c -o main cg2d.o bibaux.o -lm -lX11 && sudo ./main

