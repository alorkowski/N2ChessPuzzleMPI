/*
 * Main.cpp
 *
 * DESCRIPTION: Handles all classes related to the NQueen problem.
 *
 * Created on: May 21, 2017
 *
 * AUTHORS:
 * Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mpi.h"
#include "Handler.hpp"

#define MASTER 0

int** allocate2DInt(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}

int main(int argc, char **argv) {
    int    prank, psize;
    int    numberOfQueens = 0;
    bool   uniqueFlag = false;
    bool   printFlag = false;
    bool   gameFlag = false;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);

    numberOfQueens = 0;

    double t = MPI_Wtime();

    for(int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            numberOfQueens = atoi( argv[i+1] );
            i++;
        }
        else if (strcmp(argv[i], "-u") == 0){ uniqueFlag = true; }
        else if (strcmp(argv[i], "-p") == 0){ printFlag = true; }
        else if (strcmp(argv[i], "-g") == 0){ gameFlag = true; }
        else {}
    }
    if (numberOfQueens == 0){ numberOfQueens = 4; }

    Handler handler(numberOfQueens, psize);
    int numberOfSolutions;
    int numberOfUniqueSolutions;

    if (prank == MASTER) {
        handler.masterSolveAllSolutions();
    } else {
        handler.workerSolveAllSolutions();
    }

    MPI_Reduce(&handler.numberOfSolutions,
               &numberOfSolutions,
               1,
               MPI_INT,
               MPI_SUM,
               0,
               MPI_COMM_WORLD);

    if ( printFlag || gameFlag || uniqueFlag ) {

        int localRecvCounts[psize];
        int globalRecvCounts[psize];
        int mpiRecvDisplacements[psize];

        for (int i = 0; i < psize; ++i) {
            localRecvCounts[i] = 0;
            globalRecvCounts[i] = 0;
            mpiRecvDisplacements[i] = 0;
        }

        localRecvCounts[prank] = numberOfQueens * handler.numberOfSolutions;

        MPI_Allreduce(&(localRecvCounts[0]),
                      &(globalRecvCounts[0]),
                      psize,
                      MPI_INT,
                      MPI_SUM,
                      MPI_COMM_WORLD);

        for (int i = 0; i < psize; ++i) {
            if (i == 0 ) {
                mpiRecvDisplacements[i] = 0;
            } else {
                mpiRecvDisplacements[i] = mpiRecvDisplacements[i - 1] + globalRecvCounts[i - 1];
            }
        }

        MPI_Bcast(&numberOfSolutions, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int **allSolutions = allocate2DInt(numberOfSolutions, numberOfQueens);
        handler.convertAllSolutionVectorToArray();

        MPI_Gatherv(&(handler.allSolutionsArray[0][0]),
                    globalRecvCounts[prank],
                    MPI_INT,
                    &(allSolutions[0][0]),
                    globalRecvCounts,
                    mpiRecvDisplacements,
                    MPI_INT,
                    0,
                    MPI_COMM_WORLD);

        if (prank == MASTER) {
            handler.numberOfSolutions = numberOfSolutions;
            handler.rewriteVector(allSolutions);
        }

        if (uniqueFlag) {
            MPI_Bcast(&allSolutions[0][0],
                      numberOfSolutions * numberOfQueens,
                      MPI_INT,
                      0,
                      MPI_COMM_WORLD);
            handler.numberOfSolutions = numberOfSolutions;

            if (prank == MASTER) {
                handler.masterSolveUniqueSolutions();
            } else {
                handler.rewriteVector(allSolutions);
                handler.workerSolveUniqueSolutions();
            }

            free(allSolutions);

            MPI_Reduce(&handler.numberOfUniqueSolutions,
                       &numberOfUniqueSolutions,
                       1,
                       MPI_INT,
                       MPI_SUM,
                       0,
                       MPI_COMM_WORLD);

            for (int i = 0; i < psize; ++i) {
                localRecvCounts[i] = 0;
                globalRecvCounts[i] = 0;
                mpiRecvDisplacements[i] = 0;
            }

            localRecvCounts[prank] = numberOfQueens * handler.numberOfUniqueSolutions;

            MPI_Allreduce(&(localRecvCounts[0]),
                          &(globalRecvCounts[0]),
                          psize,
                          MPI_INT,
                          MPI_SUM,
                          MPI_COMM_WORLD);

            for (int i = 0; i < psize; ++i) {
                if (i == 0 ) {
                    mpiRecvDisplacements[i] = 0;
                } else {
                    mpiRecvDisplacements[i] = mpiRecvDisplacements[i - 1] + globalRecvCounts[i - 1];
                }
            }

            MPI_Bcast(&numberOfUniqueSolutions, 1, MPI_INT, 0, MPI_COMM_WORLD);

            int **uniqueSolutions = allocate2DInt(numberOfUniqueSolutions, numberOfQueens);

            handler.convertUniqueSolutionVectorToArray();

            MPI_Gatherv(&(handler.uniqueSolutionsArray[0][0]),
                        globalRecvCounts[prank],
                        MPI_INT,
                        &(uniqueSolutions[0][0]),
                        globalRecvCounts,
                        mpiRecvDisplacements,
                        MPI_INT,
                        0,
                        MPI_COMM_WORLD);

            if (prank == MASTER) {
                handler.numberOfUniqueSolutions = numberOfUniqueSolutions;
                handler.rewriteUniqueVector(uniqueSolutions);
            }

            free(uniqueSolutions);
        } else {
            free(allSolutions);
        }
    }

    if (prank == MASTER){
        printf("Number of solutions = %i \n",(numberOfSolutions));

        if ( printFlag || gameFlag ) {
            handler.numberOfSolutions = numberOfSolutions;
            if (uniqueFlag) {
                handler.numberOfUniqueSolutions = numberOfUniqueSolutions;
            }
        }

        if (printFlag) { handler.printAllSolutions(); }
        if (gameFlag) { handler.printAllGameBoards(); }

        if (uniqueFlag) {
            printf("Number of unique solutions = %i \n",(numberOfUniqueSolutions));
            if (printFlag) { handler.printUniqueSolutions(); }
            if (gameFlag) { handler.printUniqueGameBoards(); }
        }

        printf("Execution time = %f [s] \n",(MPI_Wtime()-t));
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
