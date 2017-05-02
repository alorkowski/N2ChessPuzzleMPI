#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "mpi.h"
#include "Master.hpp"
#include "Worker.hpp"

#define MASTER 0

int main(int argc, char **argv) {
    int    prank, psize;
    int    n;
    bool   unique_flag = false;
    bool   print_flag = false;
    bool   game_flag = false;
    bool   write_flag = false;
    bool   writeGB_flag = false;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &prank);
    MPI_Comm_size(MPI_COMM_WORLD, &psize);

    if (prank == MASTER){
        remove("list-all.dat");
        remove("list-uniq.dat");
        remove("GB-all.dat");
        remove("GB-uniq.dat");
    }

    double t = MPI_Wtime();

    if (prank == MASTER){
        for(int i = 1; i < argc; i++){
            if (strcmp(argv[i], "-n") == 0){
                n = atoi( argv[i+1] );
                if (n == 0){ n = 4; }
                i++;
            }
            else if (strcmp(argv[i], "-u") == 0){ unique_flag = true; }
            else if (strcmp(argv[i], "-p") == 0){ print_flag = true; }
            else if (strcmp(argv[i], "-g") == 0){ game_flag = true; }
            else if (strcmp(argv[i], "-w") == 0){ write_flag = true; }
            else if (strcmp(argv[i], "-wg") == 0){ writeGB_flag = true; }
            else {}
        }
        Master master(n, psize, unique_flag, print_flag, game_flag, write_flag, writeGB_flag);
    }else{
        for(int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-n") == 0) {
                n = atoi(argv[i + 1]);
                if (n == 0) { n = 4; }
            }
        }
        Worker worker(n, prank);
    }

    if (prank == MASTER){
        printf("Execution time = %f [s] \n",(MPI_Wtime()-t));
    }

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Finalize();
}
