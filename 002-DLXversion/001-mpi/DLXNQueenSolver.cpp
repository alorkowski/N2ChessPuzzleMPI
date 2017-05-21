#include "DLXNQueenSolver.hpp"

DLXNQueenSolver::DLXNQueenSolver(int numberOfQueens)
        : n_(numberOfQueens),
          problem_(6 * n_ - 2, 4 * n_ - 2) {
    assert(n_ > 0);
    int D = numberOfQueens + numberOfQueens - 1;
    for (int y = 0; y < numberOfQueens; ++y) {
        for (int x = 0; x < numberOfQueens; ++x) {
            row_data_.push_back({x, y});
            int d1 = x + y;
            int d2 = x + numberOfQueens - y - 1;
            problem_.add_row({d1,
                              D + d2,
                              D + D + x,
                              D + D + numberOfQueens + y,
                             });
        }
    }
}

DLXNQueenSolver::~DLXNQueenSolver() {}

ExactCoverProblem& DLXNQueenSolver::problem() {
    return problem_;
}

int DLXNQueenSolver::count_solutions() {
    AlgorithmDLX dlx(problem_);
    return dlx.count_solutions();
}

void DLXNQueenSolver::solve(int initialColumn,
                            int numberOfQueens,
                            std::vector<Chessboard> &allSolution,
                            int &numberOfSolutions) {
    numberOfSolutions += count_solutions();
}

