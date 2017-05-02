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
           int psize,
           bool unique_flag,
           bool print_flag,
           bool game_flag,
           bool write_flag,
           bool writeGB_flag);

    virtual ~Master();

    void solveAllSolutions();
    void shareAllSolutions();
    void solveUniqueSolutions();
    void printSolutions();
    void printGameBoard();
    void writeSolutions();
    void writeGameBoard();


    const int WORK_REQUEST = -1;
    const int WORK_COMPLETE = -2;
    const int WORK_STANDBY = -3;
    const int WORK_COUNT = -4;
    const int WORK_UNIQUE = -5;
    const int WORK_SHARE = -6;

private:

    int numberOfQueens;
    int numberOfProcessors;
    bool uniqueFlag;
    bool printFlag;
    bool gameFlag;
    bool writeFlag;
    bool writeGBFlag;

    bool subcheck(int i, int j);

    MPI_Status status;
    int        proc;
    int        workerid, msg;
    int        numberOfSolutions = 0;
    int        numberOfUniqueSolutions = 0;

    MPI_Datatype mpi_taskdetails;

    int count;
    int ucount;

    std::vector<Chessboard> allSolution;
    std::vector<Chessboard> uniqueSolution;

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