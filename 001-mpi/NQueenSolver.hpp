//! NQueenSolver.hpp
/*!
  \brief A class constructing an object capable of solving the NQueen problem via backtracking.
  \author Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
  \version 1.0
  \date  21 May 2017
  \remark Ecole Polytechnic Federal de Lausanne (EPFL)
  \remark MATH-454 Parallel and High-Performance Computing
*/

#ifndef NQUEENSOLVER_HPP_
#define NQUEENSOLVER_HPP_

#include <cstdlib>
#include <vector>
#include <iostream>
#include "Chessboard.hpp"

class NQueenSolver {
public:

    /*! A method to call the backtracking algorithm.
     *
     * \param rootColumn The column of the last place queen.
     * \param rootValue The row of the last place queen.
     * \param numberOfQueens The total number of queens to place.
     * \param col The current column.
     * \param hist A chessboard object containing an incomplete solution.
     * \param solutions A vector containing all solved solutions.
     * \param count The current number of solutions found.
     */
    virtual void solve(int rootColumn,
                       int rootValue,
                       int numberOfQueens,
                       int col,
                       Chessboard &hist,
                       std::vector<Chessboard> &solutions,
                       int &count);

    int RotaGB(int i,
               int n,
               Chessboard inmat);

    int MirrorGB(int i,
                 int n,
                 Chessboard inmat);

    bool CheckGB(int i,
                 int n,
                 int num,
                 Chessboard altmat,
                 std::vector<Chessboard> &mat);

    /*! A method to filter out unique solutions among all NQueen solutions.
     *
     * \param i The index of a solution to test.
     * \param numberOfQueens The total number of queens to place.
     * \param mat A vector containing all solutions.
     * \param umat A vector containing all unique solutions.
     * \param count The current number of unique solutions found.
     */
    void UniqGB(int i,
                int numberOfQueens,
                std::vector<Chessboard> &mat,
                std::vector<Chessboard> &umat,
                int &count);

private:
    //bool conflict(Chessboard &hist, int col, int i, int j);
};

#endif //NQUEENSOLVER_HPP_
