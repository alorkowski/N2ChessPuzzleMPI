/*
 * Handler.cpp
 *
 * DESCRIPTION: A class responsible for handling MPI protocols.
 *
 * Created on: May 21, 2017
 *
 * AUTHORS:
 * Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
 */

#include "Handler.hpp"

Handler::Handler(int n,
                 int prank,
                 int psize) : numberOfQueens(n),
                              rankOfProcessor(prank),
                              numberOfProcessors(psize - 1) {

    mpiOffsets[0] = offsetof(mpiData, task);
    mpiOffsets[1] = offsetof(mpiData, rowPlacement);
    mpiOffsets[2] = offsetof(mpiData, columnPlacement);
    mpiOffsets[3] = offsetof(mpiData, numberOfSolutions);

    MPI_Type_create_struct(nItems, blockLengths, mpiOffsets, mpiTypes, &mpiTaskDetails);
    MPI_Type_commit(&mpiTaskDetails);
}


Handler::~Handler() {
    if (allSolutionAllocated) {
        memoryAllocationTool.deallocate2DInt(allSolutionsArray);
    }
    if (uniqueSolutionAllocated) {
        memoryAllocationTool.deallocate2DInt(uniqueSolutionsArray);
    }
}


void Handler::freeMPIDerivedType() {
    MPI_Type_free(&mpiTaskDetails);
}


bool Handler::subCheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}


void Handler::masterSolveAllSolutions() {
    taskDetails.task = WORK_COUNT;
    if (numberOfProcessors > numberOfQueens) {
        for (int i = 0; i < numberOfQueens; i++) {
            for (int j = 0; j < numberOfQueens; j++) {
                if (!subCheck(i, j)) {

                    MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                    workerid = status.MPI_SOURCE;

                    taskDetails.rowPlacement = j;
                    taskDetails.columnPlacement = i;
                    MPI_Send(&taskDetails, 1, mpiTaskDetails, workerid, 0, MPI_COMM_WORLD);
                } else {
                    continue;
                }
            }
        }
    } else {
        for (int i = 0; i < numberOfQueens; i++) {
            MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            workerid = status.MPI_SOURCE;

            taskDetails.columnPlacement = i;
            taskDetails.rowPlacement = i;
            MPI_Send(&taskDetails, 1, mpiTaskDetails, workerid, 0, MPI_COMM_WORLD);
        }
    }

    for (int i = 1; i <= numberOfProcessors; i++) {
        taskDetails.task = WORK_STANDBY;
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        workerid = status.MPI_SOURCE;
        MPI_Send(&taskDetails, 1, mpiTaskDetails, workerid, 0, MPI_COMM_WORLD);
    }
}


void Handler::masterSolveAllSolutionsSparse() {
    taskDetails.task = WORK_COUNT;
    if (numberOfProcessors > numberOfQueens) {
        for (int i = 0; i < ceil((double) numberOfQueens/2); i++) {
            for (int j = 0; j < numberOfQueens; j++) {
                if (!subCheck(i, j)) {

                    if (i == floor(numberOfQueens/2)) {
                        if (numberOfQueens % 2 != 0) {
                            if (j >= ceil((double) numberOfQueens / 2)) {
                                continue;
                            }
                        }
                    }

                    MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
                    workerid = status.MPI_SOURCE;

                    taskDetails.rowPlacement = j;
                    taskDetails.columnPlacement = i;
                    MPI_Send(&taskDetails, 1, mpiTaskDetails, workerid, 0, MPI_COMM_WORLD);
                } else {
                    continue;
                }
            }
        }
    } else {
        for (int i = 0; i < ceil((double) numberOfQueens/2); i++) {
            MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
            workerid = status.MPI_SOURCE;

            taskDetails.columnPlacement = i;
            taskDetails.rowPlacement = i;
            MPI_Send(&taskDetails, 1, mpiTaskDetails, workerid, 0, MPI_COMM_WORLD);
        }
    }

    for (int i = 1; i <= numberOfProcessors; i++) {
        taskDetails.task = WORK_STANDBY;
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        workerid = status.MPI_SOURCE;
        MPI_Send(&taskDetails, 1, mpiTaskDetails, workerid, 0, MPI_COMM_WORLD);
    }
}


void Handler::workerSolveAllSolutions() {
    Chessboard chessboard(numberOfQueens);

    while (1) {
        MPI_Send(&WORK_REQUEST, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&taskDetails, 1, mpiTaskDetails, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        task = taskDetails.task;

        if (task == WORK_STANDBY) { break; }

        int rootColumn = 0;

        if (taskDetails.columnPlacement != taskDetails.rowPlacement) {
            chessboard.setState(0, taskDetails.columnPlacement);
            chessboard.setState(1, taskDetails.rowPlacement);
            rootColumn += 2;
        } else {
            chessboard.setState(0, taskDetails.columnPlacement);
            rootColumn += 1;
        }

        solver.solve(rootColumn - 1,
                     taskDetails.rowPlacement,
                     numberOfQueens,
                     rootColumn,
                     chessboard,
                     allSolutions,
                     numberOfSolutions);
    }
}


void Handler::workerSolveAllSolutionsSparse() {
    Chessboard chessboard(numberOfQueens);

    while (1) {
        MPI_Send(&WORK_REQUEST, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&taskDetails, 1, mpiTaskDetails, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        task = taskDetails.task;

        if (task == WORK_STANDBY) { break; }

        int rootColumn = 0;

        if (taskDetails.columnPlacement != taskDetails.rowPlacement) {
            chessboard.setState(0, taskDetails.columnPlacement);
            chessboard.setState(1, taskDetails.rowPlacement);
            rootColumn += 2;
        } else {
            chessboard.setState(0, taskDetails.columnPlacement);
            rootColumn += 1;
        }

        sparseSolver.solve(rootColumn - 1,
                           taskDetails.rowPlacement,
                           numberOfQueens,
                           rootColumn,
                           chessboard,
                           allSolutions,
                           numberOfSolutions);
    }
}


void Handler::solveUniqueSolutions() {
    int currentIndex = rankOfProcessor;
    while ( currentIndex < numberOfSolutions ) {

        solver.UniqGB(currentIndex,
                      numberOfQueens,
                      allSolutions,
                      uniqueSolutions,
                      numberOfUniqueSolutions);

        currentIndex += numberOfProcessors + 1;
    }
};


void Handler::reconstructSparseToDense() {
    SparseNQueenSolver solver;
    int maxIterationCount = allSolutions.size();
    for (int i = 0; i < maxIterationCount; i++) {
        solver.reconstructSparseToDense(i, numberOfQueens, allSolutions, numberOfSolutions);
    }
}


void Handler::convertAllSolutionVectorToArray() {

    allSolutionsArray = memoryAllocationTool.allocate2DInt(numberOfSolutions, numberOfQueens);
    if (numberOfSolutions == 0) { return; }
    allSolutionAllocated = true;

    for (int i = 0; i < numberOfSolutions; ++i) {
        for (int j = 0; j < numberOfQueens; ++j) {
            allSolutionsArray[i][j] = allSolutions.at(i).getState().at(j);
        }
    }
}


void Handler::convertUniqueSolutionVectorToArray() {

    uniqueSolutionsArray = memoryAllocationTool.allocate2DInt(numberOfUniqueSolutions, numberOfQueens);
    if (numberOfUniqueSolutions == 0) { return; }
    uniqueSolutionAllocated = true;

    for (int i = 0; i < numberOfUniqueSolutions; ++i) {
        for (int j = 0; j < numberOfQueens; ++j) {
            uniqueSolutionsArray[i][j] = uniqueSolutions.at(i).getState().at(j);
        }
    }
}


void Handler::convertAllSolutionArrayToVector(int** allSolutionArray) {
    allSolutions.clear();
    for(int i = 0; i < numberOfSolutions; i++) {
        Chessboard reconstructedChessboard(numberOfQueens);
        for(int j = 0; j < numberOfQueens; j++) {
            reconstructedChessboard.setState(j, allSolutionArray[i][j]);
        }
        allSolutions.push_back(reconstructedChessboard);
    }
}


void Handler::convertUniqueSolutionArrayToVector(int** uniqueSolutionArray) {
    uniqueSolutions.clear();
    for(int i = 0; i < numberOfUniqueSolutions; i++) {
        Chessboard reconstructedChessboard(numberOfQueens);
        for(int j = 0; j < numberOfQueens; j++) {
            reconstructedChessboard.setState(j, uniqueSolutionArray[i][j]);
        }
        uniqueSolutions.push_back(reconstructedChessboard);
    }
}


void Handler::printAllSolutions() {
    std::cout << std::endl << "Printing solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfSolutions; i++) {
        allSolutions.at(i).print();
    }
    std::cout << std::endl;
}


void Handler::printAllGameBoards() {
    std::cout << std::endl << "Printing solutions as in traditional chessboard format" << std::endl;
    for (int i = 0; i < numberOfSolutions; i++) {
        std::cout << std::endl << "No. " << i << std::endl << "**************" << std::endl;
        allSolutions.at(i).printGameBoard();
    }
    std::cout << std::endl;
}


void Handler::printUniqueSolutions() {
    std::cout << std::endl << "Printing unique solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfUniqueSolutions; i++) {
        uniqueSolutions.at(i).print();
    }
    std::cout << std::endl;
}


void Handler::printUniqueGameBoards() {
    std::cout << std::endl << "Printing unique solutions as in traditional chessboard format" << std::endl;
    for (int i = 0; i < numberOfUniqueSolutions; i++) {
        std::cout << std::endl << "Unique No. " << i << std::endl << "**************" << std::endl;
        uniqueSolutions.at(i).printGameBoard();
    }
    std::cout << std::endl;
}