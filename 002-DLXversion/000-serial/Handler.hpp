#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <cstdlib>
#include <iostream>
#include "Chessboard.hpp"
#include "NQueenSolver.hpp"
#include "DLXNQueenSolver.hpp"

class Handler {
public:

    Handler(int n,
            bool unique_flag,
            bool print_flag,
            bool game_flag,
            bool write_flag,
            bool writeGB_flag,
            bool dlx_flag);

    virtual ~Handler();

    void solveAllSolutions();
    void solveUniqueSolutions();
    void printSolutions();
    void printGameBoard();
    void writeSolutions();
    void writeGameBoard();

private:

    int numberOfQueens;
    bool uniqueFlag;
    bool printFlag;
    bool gameFlag;
    bool writeFlag;
    bool writeGBFlag;
    bool dlxFlag;

    bool subcheck(int i, int j);

    int numberOfSolutions = 0;
    int numberOfUniqueSolutions = 0;

    std::vector<Chessboard> allSolution;
    std::vector<Chessboard> uniqueSolution;
};


#endif //HANDLER_HPP_
