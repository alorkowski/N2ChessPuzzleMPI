#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mpi.h"

#define MASTER 0

typedef struct MPI_Data {
    int i;
    int j;
} MPI_Data;

bool subCheck(int i, int j) {
    if (j == i || abs(j - i) == 1){ return true; }
    else { return false; }
}

int main(int argc, char **argv) {
    int    prank, psize;
    int    numberOfQueens;
    bool   unique_flag = false;
    bool   print_flag = false;
    bool   game_flag = false;
    bool   write_flag = false;
    bool   writeGB_flag = false;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);

    MPI_Datatype types[4] = {MPI_INT, MPI_INT};
    MPI_Aint     offsets[2];

    offsets[0] = offsetof(MPI_Data, i);
    offsets[1] = offsetof(MPI_Data, j);

    MPI_Type_create_struct(4, {1,1}, offsets, types, &MPI_TaskDetails);
    MPI_Type_commit(&MPI_TaskDetails);

    numberOfQueens = 0;

    double t = MPI_Wtime();

    if (prank == MASTER) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-n") == 0) {
                numberOfQueens = atoi(argv[i + 1]);
                i++;
            } else if (strcmp(argv[i], "-u") == 0) { unique_flag = true; }
            else if (strcmp(argv[i], "-p") == 0) { print_flag = true; }
            else if (strcmp(argv[i], "-g") == 0) { game_flag = true; }
            else if (strcmp(argv[i], "-w") == 0) { write_flag = true; }
            else if (strcmp(argv[i], "-wg") == 0) { writeGB_flag = true; }
            else {}
        }
        if (numberOfQueens == 0) { numberOfQueens = 4; }
    }

    MPI_Data taskDetails;

    if (prank == MASTER) {
        for (int i = 0; i < numberOfQueens; i++) {
            for (int j = 0; j < numberOfQueens; j++) {
                if (!subCheck(i, j)) {
                    taskdetails.i = i;
                    taskdetails.j = j;

                } else { continue; }
            }
        }
    }





    MPI_Barrier(MPI_COMM_WORLD);

    if (prank == MASTER){
        printf("Execution time = %f [s] \n",(MPI_Wtime()-t));
    }

    MPI_Finalize();

    return 0;
}
