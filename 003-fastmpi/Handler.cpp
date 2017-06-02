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
        free(allSolutionsArray);
    }
    if (uniqueSolutionAllocated) {
        free(uniqueSolutionsArray);
    }
}


int** Handler::allocate2DInt(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++) {
        array[i] = &(data[cols * i]);
    }
    return array;
}


bool Handler::subCheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}


void Handler::solveAllSolutions(Chessboard chessboard,
                                int rootColumn,
                                int rowPlacement) {
    solver.solve(rootColumn - 1,
                 rowPlacement,
                 numberOfQueens,
                 rootColumn,
                 chessboard,
                 allSolutions,
                 numberOfSolutions);
}


void Handler::solveAllSolutionsSparse(Chessboard chessboard,
                                      int rootColumn,
                                      int rowPlacement) {

    sparseSolver.solve(rootColumn - 1,
                       rowPlacement,
                       numberOfQueens,
                       rootColumn,
                       chessboard,
                       allSolutions,
                       numberOfSolutions);
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

    allSolutionsArray = allocate2DInt(numberOfSolutions, numberOfQueens);

    allSolutionAllocated = true;

    if (numberOfSolutions == 0) { return; }

    for (int i = 0; i < numberOfSolutions; ++i) {
        for (int j = 0; j < numberOfQueens; ++j) {
            allSolutionsArray[i][j] = allSolutions.at(i).getState().at(j);
        }
    }
}


void Handler::convertUniqueSolutionVectorToArray() {

    uniqueSolutionsArray = allocate2DInt(numberOfUniqueSolutions, numberOfQueens);

    uniqueSolutionAllocated = true;

    if (numberOfUniqueSolutions == 0) { return; }

    for (int i = 0; i < numberOfUniqueSolutions; ++i) {
        for (int j = 0; j < numberOfQueens; ++j) {
            uniqueSolutionsArray[i][j] = uniqueSolutions.at(i).getState().at(j);
        }
    }
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


void Handler::rewriteUniqueVector(int **uniqueSolutionArray) {
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