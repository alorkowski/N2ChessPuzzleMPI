#ifndef MASTER_HPP_
#define MASTER_HPP_

#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <iostream>
#include <mpi.h>
#include "Chessboard.hpp"

class Master {
public:

    Master(int n,
           int psize);

    virtual ~Master();

    std::vector<Chessboard> allSolution;
    std::vector<Chessboard> uniqueSolution;

    void solveAllSolutions();
    void gatherSolutions(int &numberOf,
                         std::vector<Chessboard> &solution);
    void shareAllSolutions();
    void solveUniqueSolutions();
    void printSolutions();
    void printGameBoard();
    void writeSolutions();
    void writeGameBoard();

    int  numberOfSolutions = 0;
    int  numberOfUniqueSolutions = 0;

private:

    const int WORK_REQUEST = -1;
    const int WORK_COMPLETE = -2;
    const int WORK_STANDBY = -3;
    const int WORK_COUNT = -4;
    const int WORK_UNIQUE = -5;
    const int WORK_SHARE = -6;

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

    int count;

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

#endif /* MASTER_HPP_ */
