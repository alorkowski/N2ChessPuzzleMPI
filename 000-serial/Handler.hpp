#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include "Chessboard.hpp"

class Handler {
public:

    Handler(int n,
            bool unique_flag,
            bool print_flag,
            bool game_flag,
            bool write_flag,
            bool writeGB_flag);

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

    bool subcheck(int i, int j);

    int numberOfSolutions = 0;
    int numberOfUniqueSolutions = 0;

    std::vector<Chessboard> allSolution;
    std::vector<Chessboard> uniqueSolution;
};


#endif //HANDLER_HPP_
