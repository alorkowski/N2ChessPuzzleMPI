#!/bin/bash

f0="000-serial"
f1="001-mpi"

g++ -o nqueen-serial \
    $f0/Main-serial.cpp \
    $f0/Handler.cpp \
    $f0/Chessboard.cpp \
    $f0/NQueenSolver.cpp \
    -std=c++11 \
    -O2

mpicxx -o nqueen-mpi \
       $f1/Main.cpp \
       $f1/Handler.cpp \
       $f1/Chessboard.cpp \
       $f1/NQueenSolver.cpp \
       -std=c++11 \
       -O2
