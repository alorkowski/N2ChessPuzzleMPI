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

bool subCheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}

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
    bool   fastFlag = false;
    bool   printFlag = false;
    bool   gameFlag = false;
    bool   uniqueFlag = false;
    bool   fastUniqueFlag = false;
    bool   uniquePrintFlag = false;
    bool   uniqueGameFlag = false;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);

    if ( psize < 2 ) {
        std::cout << "Error.  Programs requires a minimum of two processors." << std::endl;
        MPI_Finalize();
        return 0;
    }

    numberOfQueens = 0;

    double t = MPI_Wtime();

    for(int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            numberOfQueens = atoi( argv[i+1] );
            i++;
        }
        else if (strcmp(argv[i], "-f") == 0){ fastFlag = true;}
        else if (strcmp(argv[i], "-u") == 0){ uniqueFlag = true; }
        else if (strcmp(argv[i], "-uf") == 0){ fastUniqueFlag = true; uniqueFlag = true;}
        else if (strcmp(argv[i], "-p") == 0){ printFlag = true; }
        else if (strcmp(argv[i], "-g") == 0){ gameFlag = true; }
        else if (strcmp(argv[i], "-up") == 0){ uniquePrintFlag = true; }
        else if (strcmp(argv[i], "-ug") == 0){ uniqueGameFlag = true; }
        else {}
    }

    if (numberOfQueens == 0){ numberOfQueens = 4; }

    Handler handler(numberOfQueens, prank, psize);

    int numberOfSolutions;
    int numberOfUniqueSolutions;

    std::vector<Chessboard> initialState;
    std::vector<int> rootColumnVector;
    std::vector<int> lastPlacementVector;

    int numberOfRow1Queens = numberOfQueens;

    if (fastFlag || fastUniqueFlag) {
        numberOfRow1Queens = ceil((double) numberOfQueens/2);
    }

    if (psize > numberOfQueens) {
        for (int i = 0; i < numberOfRow1Queens; i++) {
            for (int j = 0; j < numberOfQueens; j++) {
                if (!subCheck(i, j)) {
                    if (i == floor(numberOfQueens/2)) {
                        if (numberOfQueens % 2 != 0) {
                            if (j >= ceil(numberOfQueens / 2)) {
                                continue;
                            }
                        }
                    }
                    Chessboard chessboard(numberOfQueens);
                    chessboard.setState(0, i);
                    chessboard.setState(1, j);
                    initialState.push_back(chessboard);
                    rootColumnVector.push_back(2);
                    lastPlacementVector.push_back(j);
                }
            }
        }
    } else {
        for (int i = 0; i < numberOfRow1Queens; i++) {
            Chessboard chessboard(numberOfQueens);
            chessboard.setState(0, i);
            initialState.push_back(chessboard);
            rootColumnVector.push_back(1);
            lastPlacementVector.push_back(i);
        }
    }

    int numberOfInitialStates = initialState.size();

    for (int i = prank; i < numberOfInitialStates; i += psize) {
        if(!fastFlag && !fastUniqueFlag) {
            handler.solveAllSolutions(initialState.at(i), rootColumnVector.at(i), lastPlacementVector.at(i));
        } else {
            handler.solveAllSolutionsSparse(initialState.at(i), rootColumnVector.at(i), lastPlacementVector.at(i));
        }
    }

    if (fastFlag) {
        handler.reconstructSparseToDense();
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
        int globalRecvDisplacements[psize];

        for (int i = 0; i < psize; ++i) {
            localRecvCounts[i] = 0;
            globalRecvCounts[i] = 0;
            globalRecvDisplacements[i] = 0;
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
                globalRecvDisplacements[i] = 0;
            } else {
                globalRecvDisplacements[i] = globalRecvDisplacements[i - 1] + globalRecvCounts[i - 1];
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
                    globalRecvDisplacements,
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

            if (prank != MASTER) {
                handler.rewriteVector(allSolutions);
            }

            handler.solveUniqueSolutions();

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
                globalRecvDisplacements[i] = 0;
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
                    globalRecvDisplacements[i] = 0;
                } else {
                    globalRecvDisplacements[i] = globalRecvDisplacements[i - 1] + globalRecvCounts[i - 1];
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
                        globalRecvDisplacements,
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

    if (prank == MASTER) {
        handler.numberOfSolutions = numberOfSolutions;
        handler.numberOfUniqueSolutions = numberOfUniqueSolutions;

        if (printFlag) { handler.printAllSolutions(); }
        if (gameFlag) { handler.printAllGameBoards(); }
        if (uniquePrintFlag) {
            if (!uniqueFlag) {
                std::cout << "Warning: Program not set to solve for unique solutions.  Please run with -u."
                          << std::endl;
            } else {
                handler.printUniqueSolutions();
            }
        }
        if (uniqueGameFlag) {
            if (!uniqueFlag) {
                std::cout << "Warning: Program not set to solve for unique solutions.  Please run with -u."
                          << std::endl;
            } else {
                handler.printUniqueGameBoards();
            }
        }

        if (!fastUniqueFlag) {
            printf("Number of solutions = %i \n", (handler.numberOfSolutions));
        } else {
            if (fastFlag) {
                printf("Number of solutions = %i \n", (handler.numberOfSolutions));
            } else {
                printf("Number of solutions = %i \n", (2*handler.numberOfSolutions));
            }
        }

        if (uniqueFlag) {
            printf("Number of unique solutions = %i \n",(handler.numberOfUniqueSolutions));
        }

        printf("Execution time = %f [s] \n",(MPI_Wtime()-t));
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
