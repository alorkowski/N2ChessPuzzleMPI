#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "Handler.hpp"

#define MASTER 0

int main(int argc, char **argv) {
    int    n;
    bool   unique_flag = false;
    bool   print_flag = false;
    bool   game_flag = false;
    bool   write_flag = false;
    bool   writeGB_flag = false;

    remove("list-all.dat");
    remove("list-uniq.dat");
    remove("GB-all.dat");
    remove("GB-uniq.dat");

    clock_t tStart = clock();

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

    if (n == 0){ n = 4; }

    Handler handler(n, unique_flag, print_flag, game_flag, write_flag, writeGB_flag);

    printf("Execution time = %f [s] \n",((double)(clock() - tStart)/CLOCKS_PER_SEC));

}
