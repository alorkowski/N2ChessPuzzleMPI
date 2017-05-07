#include <iostream>
#include "Worker.hpp"

Worker::Worker(int n, int pRank) :
        numberOfQueens(n) {

    offsets[0] = offsetof(mpiData, task);
    offsets[1] = offsetof(mpiData, i);
    offsets[2] = offsetof(mpiData, j);
    offsets[3] = offsetof(mpiData, numberOfSolutions);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_taskdetails);
    MPI_Type_commit(&mpi_taskdetails);

    work();
}

Worker::~Worker() {}

int** Worker::alloc_2d_int(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}

void Worker::work() {
    while (1) {
        MPI_Send(&WORK_REQUEST, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&taskdetails, 1, mpi_taskdetails, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        task = taskdetails.task;

        if (task == WORK_COUNT) {
            solveAllSolutions();
            continue;
        } else if (task == WORK_UNIQUE) {
            solveUniqueSolutions();
            //solveUniqueSolutionsByBlock();
            continue;
        } else if (task == WORK_STANDBY) {
            MPI_Send(&count, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);

            if (count == 0){
                continue;
            }

            int **contiguousMemoryArray;

            contiguousMemoryArray = alloc_2d_int(count, numberOfQueens);

            for (int i = 0; i < count; ++i) {
                for (int j = 0; j < numberOfQueens; ++j) {
                    contiguousMemoryArray[i][j] = solutions.at(i).getState().at(j);
                }
            }

            MPI_Send(&(contiguousMemoryArray[0][0]), count * numberOfQueens, MPI_INT, 0, 0, MPI_COMM_WORLD);

            free(contiguousMemoryArray[0]);
            free(contiguousMemoryArray);
            continue;
        } else if (task == WORK_SHARE) {

            numberOfSolutions = taskdetails.numberOfSolutions;

            int** contiguousMemoryArray;
            contiguousMemoryArray = alloc_2d_int(numberOfSolutions, numberOfQueens);

            MPI_Recv(&(contiguousMemoryArray[0][0]),
                     numberOfSolutions*numberOfQueens,
                     MPI_INT,
                     0,
                     0,
                     MPI_COMM_WORLD,
                     MPI_STATUS_IGNORE);

            solutions.clear();
            allSolutions.clear();

            for (int i = 0; i < numberOfSolutions; ++i) {
                Chessboard hist(numberOfQueens);
                for (int j = 0; j < numberOfQueens; ++j) {
                    hist.setState(j, contiguousMemoryArray[i][j]);
                }
                allSolutions.push_back(hist);
            }

	    free(contiguousMemoryArray[0]);
	    free(contiguousMemoryArray);

	    count = 0;
        } else {
            break;
        }
    }
}

void Worker::solveAllSolutions() {
    Chessboard chessboard(numberOfQueens);
    NQueenSolver solver = NQueenSolver();

    int col = 0;

    if (taskdetails.i != taskdetails.j) {
        chessboard.setState(0, taskdetails.i);
        chessboard.setState(1, taskdetails.j);
        col += 2;
    } else {
        chessboard.setState(0, taskdetails.i);
        col += 1;
    }

    solver.solve(col - 1, taskdetails.j, numberOfQueens, col, chessboard, solutions, count);
}

void Worker::solveUniqueSolutions() {
    NQueenSolver solver = NQueenSolver();

    int i = taskdetails.i;
    solver.UniqGB(i, numberOfQueens, allSolutions, solutions, count);
};

void Worker::solveUniqueSolutionsByBlock() {
    NQueenSolver solver = NQueenSolver();
    int i = taskdetails.i;
    int j = taskdetails.j;

    while( i <= j) {
        solver.UniqGB(i, numberOfQueens, allSolutions, solutions, count);
        i++;
    }
};
