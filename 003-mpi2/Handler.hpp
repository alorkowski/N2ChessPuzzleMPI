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
#include <fstream>
#include <sstream>
#include <iostream>
#include "mpi.h"
#include "Chessboard.hpp"
#include "NQueenSolver.hpp"
#include "SparseNQueenSolver.hpp"
#include "MemoryAllocationTool.hpp"

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

    /*! A method to solve for all possible solutions.
     */
    void solveAllSolutions();

    /*! A method to solve for a partial set of the possible solutions using symmetry.
     */
    void solveAllSolutionsSparse();

    /*! A method to find all solutions from a set of partial solutions.
    */
    void reconstructSparseToDense();

    /*! A method to solve for all unique solutions.
     */
    void solveUniqueSolutions();

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

    /*! A method to free the derived MPI type.
     */
    void freeMPIDerivedType();

    /*! A method to transfer the contents of the allSolutions vector to an array.
    */
    void convertAllSolutionVectorToArray();

    /*! A method to transfer the contents of the uniqueSolutions vector to an array.
    */
    void convertUniqueSolutionVectorToArray();

    /*! A method to transfer the contents of an array to a vector.
     * \param allSolutionArray An array to convert to a vector.
     */
    void convertAllSolutionArrayToVector(int **allSolutionArray);

    /*! A method to transfer the contents of an array to a vector.
     * \param uniqueSolutionArray An array to convert to a vector.
     */
    void convertUniqueSolutionArrayToVector(int **uniqueSolutionArray);

    /*! An integer recording the total number of solutions.
     */
    int  numberOfSolutions = 0;

    /*! An integer recording the total number of unique solutions.
     */
    int  numberOfUniqueSolutions = 0;

    /*! A 2D integer array to store the solutions.  For MPI calls.
     */
    int** allSolutionsArray;

    /*! A 2D integer array to store the unique solutions.  For MPI calls.
     */
    int** uniqueSolutionsArray;

private:

    /*! A C++ vector storing all the solutions.
     */
    std::vector<Chessboard> allSolutions;

    /*! A C++ vector storing all the unique solutions.
     */
    std::vector<Chessboard> uniqueSolutions;

    /*! The number of Queens in the NQueen problem as well as the dimension of the chessboard.
    */
    int numberOfQueens;

    /*! The rank of the processor affiliated with this class object.
    */
    int rankOfProcessor;

    /*! The total number of processors available.
    */
    int numberOfProcessors;

    /*! An object of the NQueenSolver class with the logic to solve the problem.
    */
    NQueenSolver solver;

    /*! An object of the SparseNQueenSolver class with the logic to solve the problem.
    */
    SparseNQueenSolver sparseSolver;

    /*! An object of the MemoryAllocationTool class for dynamically allocating arrays.
    */
    MemoryAllocationTool memoryAllocationTool;

    /*! A constant integer to request work from the slave processes to master process.
     */
    const int WORK_REQUEST = -1;

    /*! A constant integer from the master process to instruct slave processes to solve the NQueen problem.
     */
    const int WORK_COUNT = -2;

    /*! A constant integer from the master process to instruct slave processes to stand idle.
     */
    const int WORK_STANDBY = -3;

    /*! An integer used to store the current task of the program.
     */
    int task;

    /*! A boolean flag to indicate whether an array has been dynamically allocated.
    */
    bool allSolutionAllocated = false;

    /*! A boolean flag to indicate whether an array has been dynamically allocated.
    */
    bool uniqueSolutionAllocated = false;

    /*! A method to check the validity of the placement of a queen.
     *
     * \param i The current column.
     * \param j The current row.
     */
    bool subCheck(int i, int j);

    /*! Standard MPI status.  Contains sender rank, tag, and count size.
    */
    MPI_Status status;

    /*! An integer to store the processor rank from the MPI status.
    */
    int workerid;

    /*! An integer to store the received MPI message.
    */
    int msg;

    /*! A derived MPI datatype.
    */
    MPI_Datatype mpiTaskDetails;

    /*! An integer that represents the number of items in the derived MPI datatype.
    */
    int nItems = 4;

    /*! An integer that represents the buffer lengths of items of the derived MPI datatype.
    */
    int blockLengths[4] = {1,1,1,1};

    /*! Specifies the MPI Datatypes of the derived MPI datatype.
    */
    MPI_Datatype mpiTypes[4] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT};

    /*! Specifies the offset of the derived MPI datatype.
    */
    MPI_Aint     mpiOffsets[4];

    /*! A type definition for a structure containing required information to pass with MPI calls.
    */
    typedef struct mpiData {
        int task;
        int columnPlacement;
        int rowPlacement;
        int numberOfSolutions;
    } mpiData;

    /*! A structure containing required information to pass with MPI calls.
    */
    mpiData taskDetails;
};

#endif /* HANDLER_HPP_ */
