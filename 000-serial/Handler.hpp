#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <math.h>
#include "Chessboard.hpp"

class Handler {
public:

    Handler(int n);

    virtual ~Handler();

    void solveAllSolutions();
    void solveAllSolutionsSparse();
    void reconstructSparseToDense();
    void printSolutions();
    void printGameBoards();
    void writeSolutions();
    void writeGameBoards();
    void solveUniqueSolutions();
    void printUniqueSolutions();
    void printUniqueGameBoards();

    int numberOfSolutions = 0;
    int numberOfUniqueSolutions = 0;

private:

    int numberOfQueens;

    bool subcheck(int i, int j);

    std::vector<Chessboard> allSolution;
    std::vector<Chessboard> uniqueSolution;
};


#endif //HANDLER_HPP_
