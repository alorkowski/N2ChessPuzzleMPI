//
// Created by Alexander Lorkowski on 5/2/17.
//

#include <cstdlib>
#include <iostream>
#include "Handler.hpp"
#include "NQueenSolver.hpp"

Handler::Handler(int n,
                 bool unique_flag,
                 bool print_flag,
                 bool game_flag,
                 bool write_flag,
                 bool writeGB_flag) :
        numberOfQueens(n),
        uniqueFlag(unique_flag),
        printFlag(print_flag),
        gameFlag(game_flag),
        writeFlag(write_flag),
        writeGBFlag(writeGB_flag) {

    solveAllSolutions();
    if (uniqueFlag) {
        solveUniqueSolutions();
    }
    if (printFlag) { printSolutions(); }
    if (gameFlag) { printGameBoard(); }
    if (writeFlag) { writeSolutions(); }
    if (writeGBFlag) { writeGameBoard(); }

    printf("Number of solutions = %i \n",(numberOfSolutions));

    if (uniqueFlag) {
        printf("Number of unique solutions = %i \n",(numberOfUniqueSolutions));
    }
}

Handler::~Handler() {}

bool Handler::subcheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}

void Handler::solveAllSolutions() {
    Chessboard chessboard(numberOfQueens);
    NQueenSolver solver = NQueenSolver();
    for (int i = 0; i < numberOfQueens; i++) {
        for (int j = 0; j < numberOfQueens; j++) {
            if (!subcheck(i, j)){
                chessboard.setState(0, i);
                chessboard.setState(1, j);
                int col = 2;
                solver.solve(j, numberOfQueens, col, chessboard, allSolution, numberOfSolutions);
            } else {
                continue;
            }
        }
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
    if (uniqueFlag) {
        std::cout << "Printing unique solutions as arrays" << std::endl;
        for (int i = 0; i < numberOfUniqueSolutions; i++) {
            uniqueSolution.at(i).print();
        }
        std::cout << std::endl;
    }
}


void Handler::printGameBoard() {
    for (int i = 0; i < numberOfSolutions; i++) {
        std::cout << std::endl << "No. " << i << std::endl << "**************" << std::endl;
        allSolution.at(i).printGameBoard();
    }
    if (uniqueFlag) {
        for (int i = 0; i < numberOfUniqueSolutions; i++) {
            std::cout << std::endl << "unique-No. " << i << std::endl << "**************" << std::endl;
            uniqueSolution.at(i).printGameBoard();
        }
    }
    std::cout << std::endl;
}


void Handler::writeSolutions() {
    for (int i = 0; i < numberOfSolutions; i++) {
        // TODO: write solutions here
        if (uniqueFlag) {
            // TODO: write unique solutions here
        }
    }
}


void Handler::writeGameBoard() {
    for (int i = 0; i < numberOfSolutions; i++) {
        // TODO: write solutions here in game board format
        if (uniqueFlag) {
            // TODO: write unique solutions here in game board format
        }
    }
}
