#include <cmath>
#include "Handler.hpp"

Handler::Handler(int n,
                 int psize) :
        numberOfQueens(n),
        numberOfProcessors(psize - 1) {

    offsets[0] = offsetof(mpiData, task);
    offsets[1] = offsetof(mpiData, i);
    offsets[2] = offsetof(mpiData, j);
    offsets[3] = offsetof(mpiData, numberOfSolutions);

    MPI_Type_create_struct(nitems, blocklengths, offsets, types, &mpi_taskdetails);
    MPI_Type_commit(&mpi_taskdetails);
}

Handler::~Handler() {}

int** Handler::alloc_2d_int(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}


bool Handler::subCheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}


void Handler::masterSolveAllSolutions() {
    taskdetails.task = WORK_COUNT;
    int threshold = (numberOfQueens - 1) * (numberOfQueens - 2);
    if (numberOfProcessors >= threshold) {
        for (int i = 0; i < numberOfQueens; i++) {
            for (int j = 0; j < numberOfQueens; j++) {
                if (!subCheck(i, j)) {

                    MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                    workerid = status.MPI_SOURCE;

                    if (msg == WORK_REQUEST) {
                        taskdetails.i = i;
                        taskdetails.j = j;
                        MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
                    }
                } else {
                    continue;
                }
            }
        }
    } else {
        for (int i = 0; i < numberOfQueens; i++) {
            MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            workerid = status.MPI_SOURCE;

            if (msg == WORK_REQUEST) {
                taskdetails.i = i;
                taskdetails.j = i;
                MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
            }
        }
    }

    for (int i = 1; i <= numberOfProcessors; i++) {
        taskdetails.task = WORK_STANDBY;
        workerid = i;
        MPI_Recv(&msg, 1, MPI_INT, workerid, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
    }
}


void Handler::masterDLXAllSolutions() {
    taskdetails.task = WORK_COUNT;
    for (int i = 0; i < numberOfQueens; i++) {
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        workerid = status.MPI_SOURCE;
        taskdetails.i = i;
        taskdetails.j = i;
        MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
    }
    for (int i = 1; i <= numberOfProcessors; i++) {
        taskdetails.task = WORK_STANDBY;
        workerid = i;
        MPI_Recv(&msg, 1, MPI_INT, workerid, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
    }
}


void Handler::masterSolveUniqueSolutions() {
    taskdetails.task = WORK_UNIQUE;
    for (int i = 0; i < numberOfSolutions; i++) {
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        workerid = status.MPI_SOURCE;

        if (msg == WORK_REQUEST){
            taskdetails.i = i;
            taskdetails.j = i;
            MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
        }
    }

    for (int i = 1; i <= numberOfProcessors; i++) {
        taskdetails.task = WORK_STANDBY;
        workerid = i;
        MPI_Recv(&msg, 1, MPI_INT, workerid, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&taskdetails, 1, mpi_taskdetails, workerid, 0, MPI_COMM_WORLD);
    }
}


void Handler::workerSolveAllSolutions() {
    NQueenSolver solver = NQueenSolver();
    Chessboard chessboard(numberOfQueens);

    while (1) {
        MPI_Send(&WORK_REQUEST, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&taskdetails, 1, mpi_taskdetails, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        task = taskdetails.task;

        if (task == WORK_STANDBY) { break; }

        int col = 0;

        if (taskdetails.i != taskdetails.j) {
            chessboard.setState(0, taskdetails.i);
            chessboard.setState(1, taskdetails.j);
            col += 2;
        } else {
            chessboard.setState(0, taskdetails.i);
            col += 1;
        }

        solver.solve(col - 1, taskdetails.j, numberOfQueens, col, chessboard, allSolutions, numberOfSolutions);
    }
}


void Handler::workerDLXAllSolutions() {
    DLXNQueenSolver solver = DLXNQueenSolver(numberOfQueens);
    while (1) {
        MPI_Send(&WORK_REQUEST, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&taskdetails, 1, mpi_taskdetails, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        task = taskdetails.task;

        if (task == WORK_STANDBY) { break; }

        solver.solve(taskdetails.i, numberOfQueens, allSolutions, numberOfSolutions);
    }
}


void Handler::workerSolveUniqueSolutions() {
    while (1) {
        MPI_Send(&WORK_REQUEST, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&taskdetails, 1, mpi_taskdetails, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        task = taskdetails.task;

        if (task == WORK_STANDBY) { break; }

        int i = taskdetails.i;

        solver.UniqGB(i, numberOfQueens, allSolutions, uniqueSolutions, numberOfUniqueSolutions);
    }
};


void Handler::collectAllSolutions() {
    int count = 0;
    allSolutions.clear();

    for (proc = 1; proc <= numberOfProcessors; proc++){
        MPI_Recv(&count, 1, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);

        if (count <= 0){ continue; }

        int **partialSolution;
        partialSolution = alloc_2d_int(count, numberOfQueens);

        MPI_Recv(&(partialSolution[0][0]), count*numberOfQueens, MPI_INT, proc, 0, MPI_COMM_WORLD, &status);

        for(int i = 0; i < count; i++) {
            Chessboard reconstructedChessboard(numberOfQueens);
            for(int j = 0; j < numberOfQueens; j++) {
                reconstructedChessboard.setState(j, partialSolution[i][j]);
            }
            allSolutions.push_back(reconstructedChessboard);
        }

        free(partialSolution[0]);
        free(partialSolution);
    }
}


void Handler::sendPartialSolutions() {

    MPI_Send(&numberOfSolutions, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    if (numberOfSolutions == 0) { return; }

    int **contiguousMemoryArray;
    contiguousMemoryArray = alloc_2d_int(numberOfSolutions, numberOfQueens);

    for (int i = 0; i < numberOfSolutions; ++i) {
        for (int j = 0; j < numberOfQueens; ++j) {
            contiguousMemoryArray[i][j] = allSolutions.at(i).getState().at(j);
        }
    }

    MPI_Send(&(contiguousMemoryArray[0][0]), numberOfSolutions * numberOfQueens, MPI_INT, 0, 0, MPI_COMM_WORLD);

    free(contiguousMemoryArray[0]);
    free(contiguousMemoryArray);
}


void Handler::rewriteVector(int **allSolutionArray) {
    allSolutions.clear();
    for(int i = 0; i < numberOfSolutions; i++) {
        Chessboard reconstructedChessboard(numberOfQueens);
        for(int j = 0; j < numberOfQueens; j++) {
            reconstructedChessboard.setState(j, allSolutionArray[i][j]);
        }
        allSolutions.push_back(reconstructedChessboard);
    }
}