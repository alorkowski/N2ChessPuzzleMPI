//! SparseNQueenSolver.hpp
/*!
  \brief A class constructing an object capable of solving the NQueen problem via backtracking.
  \author Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
  \version 1.0
  \date  21 May 2017
  \remark Ecole Polytechnic Federal de Lausanne (EPFL)
  \remark MATH-454 Parallel and High-Performance Computing
*/

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

private:

};

#endif //SPARSENQUEENSOLVER_HPP_
