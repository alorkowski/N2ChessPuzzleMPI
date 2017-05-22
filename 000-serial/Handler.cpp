//
// Created by Alexander Lorkowski on 5/2/17.
//

#include <cstdlib>
#include <iostream>
#include "Handler.hpp"
#include "NQueenSolver.hpp"

Handler::Handler(int n) : numberOfQueens(n) {}

Handler::~Handler() {}

bool Handler::subcheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}

void Handler::solveAllSolutions() {
    Chessboard chessboard(numberOfQueens);
    NQueenSolver solver = NQueenSolver();
    for (int i = 0; i < numberOfQueens; i++) {
        chessboard.setState(0, i);
        int col = 1;
        solver.solve(col - 1, i, numberOfQueens, col, chessboard, allSolution, numberOfSolutions);
    }
}


void Handler::solveUniqueSolutions() {
    NQueenSolver solver = NQueenSolver();
    for (int i = 0; i < numberOfSolutions; i++) {
        solver.UniqGB(i, numberOfQueens, allSolution, uniqueSolution, numberOfUniqueSolutions);
    }
}


void Handler::printSolutions() {
    std::cout << std::endl << "Printing solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfSolutions; i++) {
        allSolution.at(i).print();
    }
    std::cout << std::endl;
}


void Handler::printGameBoards() {
    for (int i = 0; i < numberOfSolutions; i++) {
        std::cout << std::endl << "No. " << i << std::endl << "**************" << std::endl;
        allSolution.at(i).printGameBoard();
    }
    std::cout << std::endl;
}


void Handler::printUniqueSolutions() {
    std::cout << "Printing unique solutions as arrays" << std::endl;
    for (int i = 0; i < numberOfUniqueSolutions; i++) {
        uniqueSolution.at(i).print();
    }
    std::cout << std::endl;
}


void Handler::printUniqueGameBoards() {
    for (int i = 0; i < numberOfUniqueSolutions; i++) {
        std::cout << std::endl << "unique-No. " << i << std::endl << "**************" << std::endl;
        uniqueSolution.at(i).printGameBoard();
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
