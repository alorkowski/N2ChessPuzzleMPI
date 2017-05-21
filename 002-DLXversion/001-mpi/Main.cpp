#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mpi.h"
#include "Handler.hpp"

#define MASTER 0

int** alloc_2d_int(int rows, int cols) {
    int *data = (int *)malloc(rows*cols*sizeof(int));
    int **array= (int **)malloc(rows*sizeof(int*));
    for (int i=0; i<rows; i++)
        array[i] = &(data[cols*i]);

    return array;
}

int main(int argc, char **argv) {
    int    prank, psize;
    int    numberOfQueens;
    bool   unique_flag = false;
    bool   print_flag = false;
    bool   game_flag = false;
    bool   write_flag = false;
    bool   writeGB_flag = false;
    bool   dlx_flag = false;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);

    if (prank == MASTER){
        remove("list-all.dat");
        remove("list-uniq.dat");
        remove("GB-all.dat");
        remove("GB-uniq.dat");
    }

    numberOfQueens = 0;

    double t = MPI_Wtime();

    for(int i = 1; i < argc; i++){
        if (strcmp(argv[i], "-n") == 0){
            numberOfQueens = atoi( argv[i+1] );
            i++;
        }
        else if (strcmp(argv[i], "-u") == 0){ unique_flag = true; }
        else if (strcmp(argv[i], "-p") == 0){ print_flag = true; }
        else if (strcmp(argv[i], "-g") == 0){ game_flag = true; }
        else if (strcmp(argv[i], "-w") == 0){ write_flag = true; }
        else if (strcmp(argv[i], "-wg") == 0){ writeGB_flag = true; }
        else if (strcmp(argv[i], "-dlx") == 0){ dlx_flag = true; }
        else {}
    }
    if (numberOfQueens == 0){ numberOfQueens = 4; }

    Handler handler(numberOfQueens, psize);
    int numberOfSolutions;
    int numberOfUniqueSolutions;

    if (prank == MASTER) {
        if (!dlx_flag) {
            handler.masterSolveAllSolutions();
        } else {
            handler.masterDLXAllSolutions();
        }
    } else {
        if (!dlx_flag) {
            handler.workerSolveAllSolutions();
        } else {
            handler.workerDLXAllSolutions();
        }
    }

    MPI_Reduce(&handler.numberOfSolutions,
               &numberOfSolutions,
               1,
               MPI_INT,
               MPI_SUM,
               0,
               MPI_COMM_WORLD);

    if ( print_flag == true || unique_flag == true) {
        MPI_Bcast(&numberOfSolutions, 1, MPI_INT, 0, MPI_COMM_WORLD);

        int **allSolutions = alloc_2d_int(numberOfSolutions, numberOfQueens);

        if (prank == MASTER) {
            handler.collectAllSolutions();
            for (int i = 0; i < numberOfSolutions; ++i) {
                for (int j = 0; j < numberOfQueens; ++j) {
                    allSolutions[i][j] = handler.allSolutions.at(i).getState().at(j);
                }
            }
        } else {
            handler.sendPartialSolutions();
        }

        if (unique_flag) {
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

            MPI_Reduce(&handler.numberOfUniqueSolutions,
                       &numberOfUniqueSolutions,
                       1,
                       MPI_INT,
                       MPI_SUM,
                       0,
                       MPI_COMM_WORLD);
        }
        free(allSolutions);
    }

    if (prank == MASTER){
        printf("Number of solutions = %i \n",(numberOfSolutions));

        if (unique_flag) { printf("Number of unique solutions = %i \n",(numberOfUniqueSolutions)); }

        printf("Execution time = %f [s] \n",(MPI_Wtime()-t));
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();

    return 0;
}
