//! Handler.hpp
/*!
  \brief A class responsible for containing all calls for solving the NQueen problem.
  \author Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
  \version 1.0
  \date  21 May 2017
  \remark Ecole Polytechnic Federal de Lausanne (EPFL)
  \remark MATH-454 Parallel and High-Performance Computing
*/

#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <math.h>
#include <fstream>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include "Chessboard.hpp"
#include "NQueenSolver.hpp"
#include "SparseNQueenSolver.hpp"

class Handler {
public:

    /*! A constructor to instantiate variables for Handler object.
     *
     * \param n The total number of queens.
     */
    Handler(int n);

    /*! A virtual destructor for the Handler object.
	 */
    virtual ~Handler();

    /*! A method to solve for all possible solutions.
     */
    void solveAllSolutions();

    /*! A method to solve for possible solutions using symmetry.
     */
    void solveAllSolutionsSparse();

    /*! A method to find all solutions from a set of partial solutions.
     */
    void reconstructSparseToDense();

    /*! A method to print all solutions.
     */
    void printAllSolutions();

    /*! A method to print all solutions in a traditional chessboard format.
     */
    void printAllGameBoards();

    /*! A method to solve for all unique solutions.
     */
    void solveUniqueSolutions();

    /*! A method to print all unique solutions.
     */
    void printUniqueSolutions();

    /*! A method to print all unique solutions in a traditional chessboard format.
     */
    void printUniqueGameBoards();

    /*! A method to write all solutions in array format.
     */
    void writeAllSolutions();

    /*! A method to write all solutions in a traditional chessboard format.
     */
    void writeAllGameBoards();

    /*! A method to write all unique solutions in array format.
     */
    void writeUniqueSolutions();

    /*! A method to write all unique solutions in a traditional chessboard format.
     */
    void writeUniqueGameBoards();

    /*! An integer recording the total number of solutions.
     */
    int  numberOfSolutions = 0;

    /*! An integer recording the total number of unique solutions.
     */
    int  numberOfUniqueSolutions = 0;

private:

    /*! The number of Queens in the NQueen problem as well as the dimension of the chessboard.
    */
    int numberOfQueens;

    /*! A method to check the validity of the placement of a queen.
     *
     * \param i The current column.
     * \param j The current row.
     */
    bool subCheck(int i, int j);

    /*! A C++ vector storing all the solutions.
     */
    std::vector<Chessboard> allSolutions;

    /*! A C++ vector storing all the unique solutions.
     */
    std::vector<Chessboard> uniqueSolutions;
};


#endif //HANDLER_HPP_
