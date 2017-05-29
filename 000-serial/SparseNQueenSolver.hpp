//
// Created by Alexander Lorkowski on 5/2/17.
//

#ifndef SPARSENQUEENSOLVER_HPP_
#define SPARSENQUEENSOLVER_HPP_

#include <cstdlib>
#include <vector>
#include <math.h>
#include "NQueenSolver.hpp"
#include "Chessboard.hpp"

class SparseNQueenSolver : public NQueenSolver {
public:

    void solve(int rootColumn,
               int rootValue,
               int numberOfQueens,
               int col,
               Chessboard &hist,
               std::vector<Chessboard> &solutions,
               int &count);

    void reconstructSparseToDense(int i,
                                  int numberOfQueens,
                                  std::vector<Chessboard> &mat,
                                  int &count);

private:

};

#endif //SPARSENQUEENSOLVER_HPP_
