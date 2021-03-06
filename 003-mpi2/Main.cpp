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
#include "MemoryAllocationTool.hpp"

#define MASTER 0

void usage(char* progName) {
    std::cout << std::endl
              << "Usage: mpirun -np N " << progName << " [options]" << std::endl
              << std::endl
              << "Options:" << std::endl
              << "-h     | Print this help" << std::endl
              << "-n [N] | Number of Queens" << std::endl
              << "-f     | Solve all solutions using symmetry" << std::endl
              << "-p     | Print solutions on screen" << std::endl
              << "-g     | Print solutions on screen in game board format" << std::endl
              << "-w     | Write solutions to a file" << std::endl
              << "-wg    | Write solutions to a file in game board format" << std::endl
              << "-u     | Solve for unique solutions" << std::endl
              << "-uf    | Solve for unique solutions using symmetry" << std::endl
              << "-up    | Print unique solutions on screen" << std::endl
              << "-ug    | Print unique solutions on screen in game board format" << std::endl
              << "-wu    | Write unique solutions to a file" << std::endl
              << "-wug   | Write unique solutions to a file in game board format" << std::endl
              << "-mpiio | Write out binary file of the solutions in array format" << std::endl
              << std::endl;
}


int main(int argc, char **argv) {
    int    prank, psize;
    int    numberOfQueens = 0;
    bool   fastFlag = false;
    bool   printFlag = false;
    bool   gameFlag = false;
    bool   writeFlag = false;
    bool   writeGBFlag = false;
    bool   uniqueFlag = false;
    bool   fastUniqueFlag = false;
    bool   uniquePrintFlag = false;
    bool   uniqueGameFlag = false;
    bool   writeUniqueFlag = false;
    bool   writeUniqueGBFlag = false;
    bool   mpiioWriteFlag = false;
    bool   exitFlag = false;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);

    if ( psize < 2 ) {
        std::cout << "Error.  Programs requires a minimum of two processors." << std::endl;
        usage(argv[0]);
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
        else if (strcmp(argv[i], "-w") == 0){ writeFlag = true; }
        else if (strcmp(argv[i], "-wg") == 0){ writeGBFlag = true; }
        else if (strcmp(argv[i], "-up") == 0){ uniqueFlag = true; uniquePrintFlag = true; }
        else if (strcmp(argv[i], "-ug") == 0){ uniqueFlag = true; uniqueGameFlag = true; }
        else if (strcmp(argv[i], "-wu") == 0){ uniqueFlag = true; writeUniqueFlag = true; }
        else if (strcmp(argv[i], "-wug") == 0){ uniqueFlag = true; writeUniqueGBFlag = true; }
        else if (strcmp(argv[i], "-mpiio") == 0) { mpiioWriteFlag = true; }
        else if (strcmp(argv[i], "-h") == 0){
            if(prank == MASTER) {
                usage(argv[0]);
            }
            exitFlag = true;
        }
        else {
            if(prank == MASTER) {
                usage(argv[0]);
            }
            exitFlag = true;
        }
    }

    if (numberOfQueens == 1) {
        if (prank == MASTER) {
            std::cout << "There is one solution for one queen placed on a 1x1 Chessboard." << std::endl;
        }
        exitFlag = true;
    }
    if (numberOfQueens == 2 || numberOfQueens == 3) {
        if (prank == MASTER) {
            std::cout << "There are no solutions" << std::endl;
        }
        exitFlag = true;
    }
    if (numberOfQueens == 0){
        if (prank == MASTER) {
            std::cout << "Defaulting to N = 4" << std::endl;
        }
        numberOfQueens = 4;
    }

    if (exitFlag) {
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Finalize();
        return 0;
    }

    /* TASK 1
     * Solve for all possible NQueen solutions
     */

    Handler handler(numberOfQueens, prank, psize);
    MemoryAllocationTool memoryAllocationTool = MemoryAllocationTool();

    int numberOfSolutions;
    int numberOfUniqueSolutions;

    if(!fastFlag && !fastUniqueFlag) {
        handler.solveAllSolutions();
    } else {
        handler.solveAllSolutionsSparse();
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

    if ( printFlag || gameFlag || writeFlag || writeGBFlag || uniqueFlag || mpiioWriteFlag) {

        /* TASK 2
         * Gather all solutions for printing or for finding all unique solutions
         */

        int *localRecvCounts = memoryAllocationTool.allocate1DInt(psize);
        int *globalRecvCounts = memoryAllocationTool.allocate1DInt(psize);
        int *globalRecvDisplacements = memoryAllocationTool.allocate1DInt(psize);

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

        int **allSolutions = memoryAllocationTool.allocate2DInt(numberOfSolutions, numberOfQueens);
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

        if (mpiioWriteFlag){
            MPI_Offset offset = sizeof(int)*globalRecvDisplacements[prank];
            MPI_File file;
            MPI_Status status;

            // opening one shared file
            MPI_File_open(MPI_COMM_WORLD,
                          "mpiIO-All-NQueen.bin",
                          MPI_MODE_CREATE|MPI_MODE_WRONLY,
                          MPI_INFO_NULL,
                          &file);

            MPI_File_seek(file, offset, MPI_SEEK_SET);
            MPI_File_write(file, &(handler.allSolutionsArray[0][0]), globalRecvCounts[prank], MPI_INT, &status);
            MPI_File_close(&file);
        }

        if (prank == MASTER) {
            handler.numberOfSolutions = numberOfSolutions;
            handler.convertAllSolutionArrayToVector(allSolutions);
        }

        if (uniqueFlag) {

            /* TASK 3
             * Extract all unique solutions.
             */

            MPI_Bcast(&allSolutions[0][0],
                      numberOfSolutions * numberOfQueens,
                      MPI_INT,
                      0,
                      MPI_COMM_WORLD);
            handler.numberOfSolutions = numberOfSolutions;

            if (prank != MASTER) {
                handler.convertAllSolutionArrayToVector(allSolutions);
            }

            handler.solveUniqueSolutions();

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

            int **uniqueSolutions = memoryAllocationTool.allocate2DInt(numberOfUniqueSolutions, numberOfQueens);

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

            if (mpiioWriteFlag){
                MPI_Offset offset = sizeof(int)*globalRecvDisplacements[prank];
                MPI_File file;
                MPI_Status status;

                // opening one shared file
                MPI_File_open(MPI_COMM_WORLD,
                              "mpiIO-Unique-NQueen.bin",
                              MPI_MODE_CREATE|MPI_MODE_WRONLY,
                              MPI_INFO_NULL,
                              &file);

                MPI_File_seek(file, offset, MPI_SEEK_SET);
                MPI_File_write(file, &(handler.uniqueSolutionsArray[0][0]), globalRecvCounts[prank], MPI_INT, &status);
                MPI_File_close(&file);
            }

            if (prank == MASTER) {
                handler.numberOfUniqueSolutions = numberOfUniqueSolutions;
                handler.convertUniqueSolutionArrayToVector(uniqueSolutions);
            }

            memoryAllocationTool.deallocate2DInt(uniqueSolutions);
        }

        memoryAllocationTool.deallocate2DInt(allSolutions);
        memoryAllocationTool.deallocate1DInt(localRecvCounts);
        memoryAllocationTool.deallocate1DInt(globalRecvCounts);
        memoryAllocationTool.deallocate1DInt(globalRecvDisplacements);
    }

    /* TASK 4
     * Perform user specified printing or writing tasks.
     */

    if (prank == MASTER) {
        handler.numberOfSolutions = numberOfSolutions;
        handler.numberOfUniqueSolutions = numberOfUniqueSolutions;

        if (printFlag) { handler.printAllSolutions(); }
        if (gameFlag) { handler.printAllGameBoards(); }
        if (writeFlag) { handler.writeAllSolutions(); }
        if (writeGBFlag) { handler.writeAllGameBoards(); }
        if (uniquePrintFlag) { handler.printUniqueSolutions(); }
        if (uniqueGameFlag) { handler.printUniqueGameBoards(); }
        if (writeUniqueFlag) { handler.writeUniqueSolutions(); }
        if (writeUniqueGBFlag) { handler.writeUniqueGameBoards(); }

        if (!fastUniqueFlag) {
            printf("Number of solutions = %i \n", (handler.numberOfSolutions));
        } else {
            printf("Number of solutions = %i \n", (2 * handler.numberOfSolutions));
        }

        if (uniqueFlag) {
            printf("Number of unique solutions = %i \n",(handler.numberOfUniqueSolutions));
        }

        printf("Execution time = %f [s] \n",(MPI_Wtime()-t));
    }

    handler.freeMPIDerivedType();

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
