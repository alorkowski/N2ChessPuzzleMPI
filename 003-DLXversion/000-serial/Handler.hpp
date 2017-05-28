#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <cstdlib>
#include <iostream>
#include "NQueenSolver.hpp"
#include "Chessboard.hpp"
#include "DLXNQueenSolver.hpp"

class Handler {
public:

    Handler(int n);

    virtual ~Handler();

    void solveAllSolutions();
    void printSolutions();
    void printGameBoards();
    void writeSolutions();
    void writeGameBoards();
    void solveUniqueSolutions();
    void printUniqueSolutions();
    void printUniqueGameBoards();
    void solveAllSolutionsDLX();

    int numberOfSolutions = 0;
    int numberOfUniqueSolutions = 0;

private:

    int numberOfQueens;

    bool subcheck(int i, int j);

    std::vector<Chessboard> allSolution;
    std::vector<Chessboard> uniqueSolution;
};


#endif //HANDLER_HPP_
