MPI NQueen Backtracking Solver
==============================

This is a serial and MPI implementation of the NQueen backtracking algorithm
written in C++. 


Implementation
==============

The implementation consists of a few classes defined under 000-serial for the
serial code and 001-mpi for the MPI version.

Also included is the code with Dancing Links incorporated. The sources for
this is located in 002-DLXversion. The algorithm implemented is the
Algorithm DLX from Knuth's Dancing Links paper for the exact cover problem.
The original paper can be found in the following link:

http://arxiv.org/pdf/cs/0011047v1.pdf

All the source code in the 002-DLXversion/002-DLXinclude directory are not my
source code. After implementing the backtracking algorithm, I was intrigued
by the efficiency of the DLX algorithm and so I incorporated it into my code
to see its speed in practice. The original source code can be found in the
following GIT repository:

https://github.com/jlaire/dlx-cpp


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
  |   -p     | n/a              | Print solutions as an array                    |
  |   -gb    | n/a              | Print solutions in chessboard format           |
  |   -w     | n/a              | Write solutions to a file as an array          |
  |   -wg    | n/a              | Write solutions to a file in Chessboard format |
  |   -dlx   | n/a              | Solve using Dancking Links algorithm (*)       |

(*) Only available in the DLX version of the code.

Examples:

./build/nqueen-serial -n 10

mpirun -np 4 ./build/nqueen-mpi -n 10

./build/nqueen-serial -n 10 -u -p

mpirun -np 4 ./build/nqueen-mpi	-n 10 -u -gb

./build/nqueen-serial -n 10

./002-DLXversion/build/nqueen-serial -n 10 -dlx