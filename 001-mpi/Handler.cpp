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


void Handler::masterSolveUniqueSolutions() {
    taskDetails.task = WORK_COUNT;
    for (int i = 0; i < numberOfSolutions; i++) {
        MPI_Recv(&msg, 1, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &status);
        workerid = status.MPI_SOURCE;

        taskDetails.rowPlacement = i;
        taskDetails.columnPlacement = i;
        MPI_Send(&taskDetails, 1, mpiTaskDetails, workerid, 0, MPI_COMM_WORLD);
    }

    for (int i = 1; i <= numberOfProcessors; i++) {
        taskDetails.task = WORK_STANDBY;
        workerid = i;
        MPI_Recv(&msg, 1, MPI_INT, workerid, 0, MPI_COMM_WORLD, &status);
        MPI_Send(&taskDetails, 1, mpiTaskDetails, workerid, 0, MPI_COMM_WORLD);
    }
}


void Handler::workerSolveUniqueSolutions() {
    while (1) {
        MPI_Send(&WORK_REQUEST, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        MPI_Recv(&taskDetails, 1, mpiTaskDetails, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        task = taskDetails.task;

        if (task == WORK_STANDBY) { break; }

        int columnIndex = taskDetails.columnPlacement;

        solver.UniqGB(columnIndex,
                      numberOfQueens,
                      allSolutions,
                      uniqueSolutions,
                      numberOfUniqueSolutions);
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

    if (numberOfSolutions > 0) {
        allSolutionsArray = memoryAllocationTool.allocate2DInt(numberOfSolutions, numberOfQueens);
    } else {
        allSolutionsArray = memoryAllocationTool.allocate2DEmpty();
    }
    allSolutionAllocated = true;
    if (numberOfSolutions == 0) { return; }

    for (int i = 0; i < numberOfSolutions; ++i) {
        for (int j = 0; j < numberOfQueens; ++j) {
            allSolutionsArray[i][j] = allSolutions.at(i).getState().at(j);
        }
    }
}


void Handler::convertUniqueSolutionVectorToArray() {

    if (numberOfUniqueSolutions > 0) {
        uniqueSolutionsArray = memoryAllocationTool.allocate2DInt(numberOfUniqueSolutions, numberOfQueens);
    } else {
        uniqueSolutionsArray = memoryAllocationTool.allocate2DEmpty();
    }
    uniqueSolutionAllocated = true;
    if (numberOfUniqueSolutions == 0) { return; }

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
        std::cout << std::endl << "No. " << ( i + 1 ) << std::endl << "**************" << std::endl;
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
        std::cout << std::endl << "Unique No. " << ( i + 1 ) << std::endl << "**************" << std::endl;
        uniqueSolutions.at(i).printGameBoard();
    }
    std::cout << std::endl;
}


void Handler::writeAllSolutions() {
    time_t t = time(0);
    struct tm * now = localtime( & t );

    std::stringstream ss;
    ss << (now->tm_year + 1900)
       << (now->tm_mon + 1)
       << (now->tm_mday)  << '-'
       << (now->tm_hour)
       << (now->tm_min)
       << (now->tm_sec);

    std::ofstream outputFile;
    outputFile.open("array-NQueen-" + std::to_string(numberOfQueens) + "-" + ss.str() + ".dat");
    std::cout << "Printing solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfSolutions; i++) {
        allSolutions.at(i).write(outputFile);
    }
    outputFile.close();
}


void Handler::writeAllGameBoards() {
    time_t t = time(0);
    struct tm * now = localtime( & t );

    std::stringstream ss;
    ss << (now->tm_year + 1900)
       << (now->tm_mon + 1)
       << (now->tm_mday)  << '-'
       << (now->tm_hour)
       << (now->tm_min)
       << (now->tm_sec);

    std::ofstream outputFile;
    outputFile.open("gb-NQueen-" + std::to_string(numberOfQueens) + "-" + ss.str() + ".dat");
    for (int i = 0; i < numberOfSolutions; i++) {
        outputFile << std::endl << "No. " << (i + 1) << std::endl << "**************" << std::endl;
        allSolutions.at(i).writeGameBoard(outputFile);
    }
    outputFile.close();
}


void Handler::writeUniqueSolutions() {
    time_t t = time(0);
    struct tm * now = localtime( & t );

    std::stringstream ss;
    ss << (now->tm_year + 1900)
       << (now->tm_mon + 1)
       << (now->tm_mday)  << '-'
       << (now->tm_hour)
       << (now->tm_min)
       << (now->tm_sec);

    std::ofstream outputFile;
    outputFile.open("array-NQueen-" + std::to_string(numberOfQueens) + "-" + ss.str() + ".dat");
    std::cout << "Printing unique solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfUniqueSolutions; i++) {
        uniqueSolutions.at(i).write(outputFile);
    }
    outputFile.close();
}


void Handler::writeUniqueGameBoards() {
    time_t t = time(0);
    struct tm * now = localtime( & t );

    std::stringstream ss;
    ss << (now->tm_year + 1900)
       << (now->tm_mon + 1)
       << (now->tm_mday)  << '-'
       << (now->tm_hour)
       << (now->tm_min)
       << (now->tm_sec);

    std::ofstream outputFile;
    outputFile.open("gb-NQueen-" + std::to_string(numberOfQueens) + "-" + ss.str() + ".dat");
    for (int i = 0; i < numberOfUniqueSolutions; i++) {
        outputFile << std::endl << "No. " << (i + 1) << std::endl << "**************" << std::endl;
        uniqueSolutions.at(i).writeGameBoard(outputFile);
    }
    outputFile.close();
}
