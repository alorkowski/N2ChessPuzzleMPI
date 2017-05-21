#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include "Chessboard.hpp"
#include "NQueenSolver.hpp"
#include "DLXNQueenSolver.hpp"

class Handler {
public:

    Handler(int n,
           int psize);

    virtual ~Handler();

    std::vector<Chessboard> allSolutions;
    std::vector<Chessboard> uniqueSolutions;
    int** allSolutionsArray;
    int** uniqueSolutionsArray;

    void masterSolveAllSolutions();
    void masterDLXAllSolutions();
    void masterSolveUniqueSolutions();

    void workerSolveAllSolutions();
    void workerDLXAllSolutions();
    void workerSolveUniqueSolutions();

    void collectAllSolutions();
    void sendPartialSolutions();
    void rewriteVector(int **allSolutionArray);

    int  numberOfSolutions = 0;
    int  numberOfUniqueSolutions = 0;

private:

    NQueenSolver solver;

    const int WORK_REQUEST = -1;
    const int WORK_COMPLETE = -2;
    const int WORK_STANDBY = -3;
    const int WORK_COUNT = -4;
    const int WORK_UNIQUE = -5;
    const int WORK_SHARE = -6;

    int task;

    int numberOfQueens;
    int numberOfProcessors;
    bool uniqueFlag;
    bool printFlag;
    bool gameFlag;
    bool writeFlag;
    bool writeGBFlag;

    bool subCheck(int i, int j);

    MPI_Status status;
    int        proc;
    int        workerid, msg;

    MPI_Datatype mpi_taskdetails;

    int          nitems = 4;
    int          blocklengths[4] = {1,1,1,1};
    MPI_Datatype types[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Aint     offsets[4];

    typedef struct mpiData {
        int task;
        int i;
        int j;
        int numberOfSolutions;
    } mpiData;

    mpiData taskdetails;

    int** alloc_2d_int(int rows, int cols);

};

#endif /* HANDLER_HPP_ */
