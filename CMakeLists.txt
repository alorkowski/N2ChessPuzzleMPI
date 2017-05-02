cmake_minimum_required(VERSION 3.6)
project(src)

set(CMAKE_CXX_STANDARD 11)
include_directories(/usr/include/mpi/)

set(SOURCE_FILES
        Main.cpp
        Master.cpp
        Master.hpp
        Worker.cpp
        Worker.hpp
        Chessboard.cpp
        Chessboard.hpp
        NQueenSolver.cpp
        NQueenSolver.hpp Main-serial.cpp Handler.cpp Handler.hpp)

add_executable(src ${SOURCE_FILES})