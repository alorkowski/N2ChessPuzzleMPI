#ifndef DLXNQUEENSOLVER_HPP_
#define DLXNQUEENSOLVER_HPP_

#include <vector>
#include <assert.h>
#include "../002-DLXinclude/AlgorithmDLX.hpp"
#include "../002-DLXinclude/ExactCoverProblem.hpp"
#include "Chessboard.hpp"

class DLXNQueenSolver{
public:
	DLXNQueenSolver(int numberOfQueens,
					std::vector<Chessboard> &allSolution,
					int &numberOfSolutions);

	~DLXNQueenSolver();

    int count_solutions();
	std::vector<std::vector<int> > find_solutions();

	ExactCoverProblem& problem();

private:
    struct Position {
		int x;
		int y;
    };

    int n_;
	ExactCoverProblem problem_;
    std::vector<Position> row_data_;
};

#endif