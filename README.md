MPI NQueen Backtracking Solver
==============================

This is a serial and MPI implementation of the NQueen backtracking algorithm
written in C++. 


Implementation
==============

The implementation consists of a few classes defined under 000-serial for the
serial code and 001-mpi for the MPI version.


How To Compile
==============

Executables can be built using make. They will be located in the build/
directory.  If you want to run the DLX version, go into the DLXversion/
directory and run make.


How To Run
==========

The code provides a few command line options.  Either include a flag or a flag
followed by a value accoring to the table below:

  | CMD Flag | Argument         | What does it do?                               |
  | :------: | :--------------: | :--------------------------------------------: | 
  |   -n     | number of queens | Sets problem size                              |
  |   -u     | n/a              | Solve for unique solutions                     |
  |   -uf    | n/a              | Solve for unique solutions using symmetry (*)  |
  |   -p     | n/a              | Print solutions as an array                    |
  |   -g     | n/a              | Print solutions in chessboard format           |
  |   -up    | n/a              | Print unique solutions as an array             |
  |   -ug    | n/a              | Print unique solutions in chessboard format    |
  |   -w     | n/a              | Write solutions to a file as an array          |
  |   -wg    | n/a              | Write solutions to a file in Chessboard format |

(*) Cannot print out all non-unique solutions ( -p and -g incompatible )

Examples:

./build/nqueen-serial -n 10

mpirun -np 4 ./build/nqueen-mpi -n 10

./build/nqueen-serial -n 10 -u -p

mpirun -np 4 ./build/nqueen-mpi	-n 10 -u -gb

./build/nqueen-serial -n 10
