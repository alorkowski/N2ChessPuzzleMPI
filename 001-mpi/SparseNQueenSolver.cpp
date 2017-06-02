#include "SparseNQueenSolver.hpp"

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
                int halfBoardLength = floor(numberOfQueens/2);
                if (hist.getState().at(rootColumn) == halfBoardLength) {
                    if (j == rootColumn + 1) {
                        if (k >= ceil((double) numberOfQueens / 2)) {
                            continue;
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


void SparseNQueenSolver::reconstructSparseToDense(int index,
                                                  int numberOfQueens,
                                                  std::vector<Chessboard> &mat,
                                                  int &count) {
    Chessboard matM(numberOfQueens);

    // Reflection of the original Game Board.
    for (int r = 0; r < numberOfQueens ; r++){matM.setState(r, MirrorGB(r, numberOfQueens, mat.at(index)));}

    mat.push_back(matM);
    count++;
}