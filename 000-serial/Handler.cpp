//
// Created by Alexander Lorkowski on 5/2/17.
//

#include <cstdlib>
#include <iostream>
#include "Handler.hpp"
#include "NQueenSolver.hpp"
#include "SparseNQueenSolver.hpp"

Handler::Handler(int n) : numberOfQueens(n) {}


Handler::~Handler() {}


bool Handler::subCheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}


void Handler::solveAllSolutions() {
    Chessboard chessboard(numberOfQueens);
    NQueenSolver solver;
    for (int i = 0; i < numberOfQueens; i++) {
        chessboard.setState(0, i);
        int col = 1;
        solver.solve(col - 1, i, numberOfQueens, col, chessboard, allSolutions, numberOfSolutions);
    }
}


void Handler::solveAllSolutionsSparse() {
    Chessboard chessboard(numberOfQueens);
    SparseNQueenSolver solver;
    for (int i = 0; i < ceil((float) numberOfQueens/2); i++) {
        chessboard.setState(0, i);
        int col = 1;
        solver.solve(col - 1, i, numberOfQueens, col, chessboard, allSolutions, numberOfSolutions);
    }
}


void Handler::solveUniqueSolutions() {
    NQueenSolver solver;
    for (int i = 0; i < numberOfSolutions; i++) {
        solver.UniqGB(i, numberOfQueens, allSolutions, uniqueSolutions, numberOfUniqueSolutions);
    }
}


void Handler::reconstructSparseToDense() {
    SparseNQueenSolver solver;
    int maxIterationCount = allSolutions.size();
    for (int i = 0; i < maxIterationCount; i++) {
        solver.reconstructSparseToDense(i, numberOfQueens, allSolutions, numberOfSolutions);
    }
}


void Handler::printSolutions() {
    std::cout << std::endl << "Printing solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfSolutions; i++) {
        allSolutions.at(i).print();
    }
    std::cout << std::endl;
}


void Handler::printGameBoards() {
    for (int i = 0; i < numberOfSolutions; i++) {
        std::cout << std::endl << "No. " << (i + 1) << std::endl << "**************" << std::endl;
        allSolutions.at(i).printGameBoard();
    }
    std::cout << std::endl;
}


void Handler::printUniqueSolutions() {
    std::cout << "Printing unique solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfUniqueSolutions; i++) {
        uniqueSolutions.at(i).print();
    }
    std::cout << std::endl;
}


void Handler::printUniqueGameBoards() {
    for (int i = 0; i < numberOfUniqueSolutions; i++) {
        std::cout << std::endl << "unique-No. " << (i + 1) << std::endl << "**************" << std::endl;
        uniqueSolutions.at(i).printGameBoard();
    }
    std::cout << std::endl;
}


void Handler::writeSolutions() {
    for (int i = 0; i < numberOfSolutions; i++) {
        // TODO: write solutions here
    }
}


void Handler::writeGameBoards() {
    for (int i = 0; i < numberOfSolutions; i++) {
        // TODO: write solutions here in game board format
    }
}
