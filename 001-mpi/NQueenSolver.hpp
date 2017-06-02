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

    /*! A method to filter out unique solutions among all NQueen solutions.
     *
     * \param indexToTest The index of a solution to test.
     * \param numberOfQueens The total number of queens to place.
     * \param mat A vector containing all solutions.
     * \param umat A vector containing all unique solutions.
     * \param count The current number of unique solutions found.
     */
    void UniqGB(int indexToTest,
                int numberOfQueens,
                std::vector<Chessboard> &mat,
                std::vector<Chessboard> &umat,
                int &count);

protected:
    /*! A method to call the backtracking algorithm.
     *
     * \param index The index of chessboard object to rotate.
     * \param n The number of queens.
     * \param inmat The chessboard object to rotate.
     */
    int RotaGB(int index,
               int n,
               Chessboard inmat);

    /*! A method to call the backtracking algorithm.
     *
     * @param index The index of chessboard object to mirror.
     * @param n The number of queens.
     * @param inmat The chessboard object to mirror.
     *
     */
    int MirrorGB(int index,
                 int n,
                 Chessboard inmat);

    /*! A method to test for uniqueness.
     *
     * @param index The index of chessboard object to test for uniqueness.
     * @param n The number of queens.
     * @param altmat A chessboard object to test for uniqueness.
     * @param mat A vector of Chessboard objects to compare against.
     * @return True if the chessboard object is unique.  False otherwise.
     */
    bool CheckGB(int index,
                 int n,
                 Chessboard altmat,
                 std::vector<Chessboard> &mat);
};

#endif //NQUEENSOLVER_HPP_
