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


void Handler::solveAllSolutions() {
    std::vector<Chessboard> initialState;
    std::vector<int> rootColumnVector;
    std::vector<int> lastPlacementVector;

    int numberOfRow1Queens = numberOfQueens;

    if (numberOfProcessors + 1 > numberOfQueens) {
        for (int i = 0; i < numberOfRow1Queens; i++) {
            for (int j = 0; j < numberOfQueens; j++) {
                if (!subCheck(i, j)) {
                    if (i == floor(numberOfQueens/2)) {
                        if (numberOfQueens % 2 != 0) {
                            if (j >= ceil(numberOfQueens / 2)) {
                                continue;
                            }
                        }
                    }
                    Chessboard chessboard(numberOfQueens);
                    chessboard.setState(0, i);
                    chessboard.setState(1, j);
                    initialState.push_back(chessboard);
                    rootColumnVector.push_back(2);
                    lastPlacementVector.push_back(j);
                }
            }
        }
    } else {
        for (int i = 0; i < numberOfRow1Queens; i++) {
            Chessboard chessboard(numberOfQueens);
            chessboard.setState(0, i);
            initialState.push_back(chessboard);
            rootColumnVector.push_back(1);
            lastPlacementVector.push_back(i);
        }
    }

    int numberOfInitialStates = initialState.size();

    for (int i = rankOfProcessor; i < numberOfInitialStates; i += numberOfProcessors + 1) {
        int rootColumn = rootColumnVector.at(i);
        int rowPlacement = lastPlacementVector.at(i);

        solver.solve(rootColumn - 1,
                     rowPlacement,
                     numberOfQueens,
                     rootColumn,
                     initialState.at(i),
                     allSolutions,
                     numberOfSolutions);
    }
}


void Handler::solveAllSolutionsSparse() {
    std::vector<Chessboard> initialState;
    std::vector<int> rootColumnVector;
    std::vector<int> lastPlacementVector;

    int numberOfRow1Queens = ceil((double) numberOfQueens/2);

    if (numberOfProcessors + 1 > numberOfQueens) {
        for (int i = 0; i < numberOfRow1Queens; i++) {
            for (int j = 0; j < numberOfQueens; j++) {
                if (!subCheck(i, j)) {
                    if (i == floor(numberOfQueens/2)) {
                        if (numberOfQueens % 2 != 0) {
                            if (j >= ceil(numberOfQueens / 2)) {
                                continue;
                            }
                        }
                    }
                    Chessboard chessboard(numberOfQueens);
                    chessboard.setState(0, i);
                    chessboard.setState(1, j);
                    initialState.push_back(chessboard);
                    rootColumnVector.push_back(2);
                    lastPlacementVector.push_back(j);
                }
            }
        }
    } else {
        for (int i = 0; i < numberOfRow1Queens; i++) {
            Chessboard chessboard(numberOfQueens);
            chessboard.setState(0, i);
            initialState.push_back(chessboard);
            rootColumnVector.push_back(1);
            lastPlacementVector.push_back(i);
        }
    }

    int numberOfInitialStates = initialState.size();

    for (int i = rankOfProcessor; i < numberOfInitialStates; i += numberOfProcessors + 1) {
        int rootColumn = rootColumnVector.at(i);
        int rowPlacement = lastPlacementVector.at(i);
        sparseSolver.solve(rootColumn - 1,
                           rowPlacement,
                           numberOfQueens,
                           rootColumn,
                           initialState.at(i),
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
}


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
