#!/bin/bash

g++ -o nqueen-serial Main-serial.cpp Handler.cpp Chessboard.cpp NQueenSolver.cpp -std=c++11
mpicxx -o nqueen-mpi Main.cpp Master.cpp Worker.cpp Chessboard.cpp NQueenSolver.cpp -std=c++11
