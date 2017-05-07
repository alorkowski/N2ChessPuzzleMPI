#!/bin/bash

f0="000-serial"
f1="001-sendRecv"

g++ -o nqueen-serial \
    $f0/Main-serial.cpp \
    $f0/Handler.cpp \
    $f0/Chessboard.cpp \
    $f0/NQueenSolver.cpp \
    -std=c++11

mpicxx -o nqueen-sendRecv \
       $f1/Main.cpp \
       $f1/Master.cpp \
       $f1/Worker.cpp \
       $f1/Chessboard.cpp \
       $f1/NQueenSolver.cpp \
       -std=c++11
