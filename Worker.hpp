//
// Created by Alexander Lorkowski on 5/1/17.
//

#ifndef WORKER_HPP_
#define WORKER_HPP_

#include <mpi.h>
#include <stddef.h>
#include <stdlib.h>
#include <vector>
#include "Chessboard.hpp"
#include "NQueenSolver.hpp"

class Worker {
public:

    Worker(int n, int pRank);

    virtual ~Worker();

    void work();

    void solveAllSolutions();
    void solveUniqueSolutions();

    const int WORK_REQUEST = -1;
    const int WORK_COMPLETE = -2;
    const int WORK_STANDBY = -3;
    const int WORK_COUNT = -4;
    const int WORK_UNIQUE = -5;
    const int WORK_SHARE = -6;

private:
    int numberOfQueens;

    MPI_Status status;
    int        proc, task;
    int        workerid, msg;
    int        numberOfSolutions = 0;
    int        numberOfUniqueSolutions = 0;

    int          nitems = 4;
    int          blocklengths[4] = {1,1,1,1};
    MPI_Datatype types[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Aint     offsets[4];

    MPI_Datatype mpi_taskdetails;

    int count = 0;

    typedef struct mpiData {
        int task;
        int i;
        int j;
        int numberOfSolutions;
    } mpiData;

    mpiData taskdetails;

    std::vector<Chessboard> solutions;
    std::vector<Chessboard> allSolutions;

    int** alloc_2d_int(int rows, int cols);

};

#endif //WORKER_HPP_
