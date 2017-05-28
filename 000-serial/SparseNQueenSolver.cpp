#include "SparseNQueenSolver.hpp"
#include <iostream>

// i is the row index and j is the column index
#define conflict(i, j, col) (hist.getState().at(j) == i || abs(hist.getState().at(j) - i) == col - j)

void SparseNQueenSolver::solve(int rootColumn,
                               int rootValue,
                               int numberOfQueens,
                               int col,
                               Chessboard &hist,
                               std::vector<Chessboard> &solutions,
                               int &count) {
    if (col == numberOfQueens) {
        count++;
        solutions.push_back(hist);
        return;
    }

    /* Backtracking Algorithm
     * Continue until all initial positions of the Queen
     * on the first column are explored.
     * */
    for (int k = 0, j = 0; k < numberOfQueens; k++) {
        for (j = 0; j < col && !conflict(k, j, col); j++);
        if (j < col) {
            continue;
        }
        if (rootColumn == 0) {
            if (numberOfQueens % 2 != 0) {
                int halfBoardLength = ceil(numberOfQueens/2);
                if (hist.getState().at(rootColumn) == halfBoardLength) {
                    if (j == rootColumn + 1) {
                        if (k >= ceil(numberOfQueens / 2)) {
                            break;
                        }
                    }
                }
            }
        }
        hist.setState(col,k);
        if ( hist.getState().at(rootColumn) != rootValue ){
            return;
        }
        solve(rootColumn, rootValue, numberOfQueens, col + 1, hist, solutions, count);
    }
}
