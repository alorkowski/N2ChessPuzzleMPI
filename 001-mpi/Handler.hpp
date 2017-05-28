//! Handler.hpp
/*!
  \brief A class responsible for delegating tasks between MPI processes.
  \author Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
  \version 1.0
  \date  21 May 2017
  \remark Ecole Polytechnic Federal de Lausanne (EPFL)
  \remark MATH-454 Parallel and High-Performance Computing
*/

#ifndef HANDLER_HPP_
#define HANDLER_HPP_

#include <vector>
#include <stddef.h>
#include <stdint.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include "mpi.h"
#include "Chessboard.hpp"
#include "NQueenSolver.hpp"
#include "SparseNQueenSolver.hpp"

class Handler {
public:

    /*! A constructor to instantiate variables for Handler object.
     *
     * \param n The total number of queens.
     * \param prank The rank of the processor.
     * \param psize The number of processors.
     */
    Handler(int n,
            int prank,
            int psize);

    /*! A virtual destructor for the Handler object.
	 */
    virtual ~Handler();

    /*! A method to solve for all possible solutions specific to processor Rank 0.
     */
    void masterSolveAllSolutions();

    /*! A method to solve for some of the possible solutions specific to processor Rank 0.
     */
    void masterSolveAllSolutionsSparse();

    /*! A method to solve for all possible solutions specific to any processor not Rank 0.
     */
    void workerSolveAllSolutions();

    /*! A method to solve for some of the possible solutions specific to any processor not Rank 0.
     */
    void workerSolveAllSolutionsSparse();

    /*! A method to solve for all unique solutions.
     */
    void solveUniqueSolutions();

    /*! A method to transfer the contents of an array to a vector.
     */
    void rewriteVector(int **allSolutionArray);

    /*! A method to transfer the contents of an array to a vector.
     */
    void rewriteUniqueVector(int **uniqueSolutionArray);

    /*! A method to print all solutions.
     */
    void printAllSolutions();

    /*! A method to print all solutions in a traditional chessboard format.
     */
    void printAllGameBoards();

    /*! A method to print all unique solutions.
     */
    void printUniqueSolutions();

    /*! A method to print all unique solutions in a traditional chessboard format.
     */
    void printUniqueGameBoards();

    void convertAllSolutionVectorToArray();
    void convertUniqueSolutionVectorToArray();

    int  numberOfSolutions = 0;
    int  numberOfUniqueSolutions = 0;
    std::vector<Chessboard> allSolutions;
    std::vector<Chessboard> uniqueSolutions;
    int** allSolutionsArray;
    int** uniqueSolutionsArray;

protected:

    const int WORK_REQUEST = -1;
    const int WORK_COMPLETE = -2;
    const int WORK_STANDBY = -3;
    const int WORK_COUNT = -4;
    const int WORK_UNIQUE = -5;
    const int WORK_SHARE = -6;

    int task;

    int numberOfQueens;
    int rankOfProcessor;
    int numberOfProcessors;

    bool subCheck(int i, int j);

    MPI_Status status;
    int        proc;
    int        workerid, msg;

    MPI_Datatype mpiTaskDetails;

    int          nItems = 4;
    int          blockLengths[4] = {1,1,1,1};
    MPI_Datatype mpiTypes[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};
    MPI_Aint     mpiOffsets[4];

    typedef struct mpiData {
        int task;
        int columnPlacement;
        int rowPlacement;
        int numberOfSolutions;
    } mpiData;

    mpiData taskDetails;

    int** allocate2DInt(int rows, int cols);

    bool allSolutionAllocated = false;
    bool uniqueSolutionAllocated = false;

private:
    NQueenSolver solver;
    SparseNQueenSolver sparseSolver;

};

#endif /* HANDLER_HPP_ */
