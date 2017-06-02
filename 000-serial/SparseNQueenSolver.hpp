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

    /*! A method to call the backtracking algorithm utilizing symmetry.
     *
     * \param rootColumn The column of the last place queen.
     * \param rootValue The row of the last place queen.
     * \param numberOfQueens The total number of queens to place.
     * \param col The current column.
     * \param hist A chessboard object containing an incomplete solution.
     * \param solutions A vector containing all solved solutions.
     * \param count The current number of solutions found.
     */
    void solve(int rootColumn,
               int rootValue,
               int numberOfQueens,
               int col,
               Chessboard &hist,
               std::vector<Chessboard> &solutions,
               int &count);

    /*! A method to use symmetry to obtain all solutions from a set of partial solutions.
     *
     * @param index The index of which to apply symmetry to obtain another valid NQueen solutions.
     * @param numberOfQueens The total number of queens to place.
     * @param mat A chessboard object containing a complete, valid solution.
     * @param count The current number of solutions found.
     */
    void reconstructSparseToDense(int index,
                                  int numberOfQueens,
                                  std::vector<Chessboard> &mat,
                                  int &count);

private:

};

#endif //SPARSENQUEENSOLVER_HPP_
