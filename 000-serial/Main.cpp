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
#include <ctime>
#include <iostream>
#include "Handler.hpp"

void usage(char* progName) {
    std::cout << std::endl
              << "Usage: " << progName << " [options]" << std::endl
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
              << std::endl;
}


int main(int argc, char **argv) {
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

    clock_t tStart = clock();

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
        else if (strcmp(argv[i], "-up") == 0){ uniquePrintFlag = true; }
        else if (strcmp(argv[i], "-ug") == 0){ uniqueGameFlag = true; }
        else if (strcmp(argv[i], "-wu") == 0){ writeUniqueFlag = true; }
        else if (strcmp(argv[i], "-wug") == 0){ writeUniqueGBFlag = true; }
        else if (strcmp(argv[i], "-h") == 0){
            usage(argv[0]);
            return 0;
        }
        else {
            usage(argv[0]);
            return 0;
        }
    }

    if (numberOfQueens <= 0){ numberOfQueens = 4; }

    Handler handler(numberOfQueens);

    if(!fastFlag && !fastUniqueFlag) {
        handler.solveAllSolutions();
    } else {
        handler.solveAllSolutionsSparse();
    }

    if(fastFlag) {
         handler.reconstructSparseToDense();
    }

    if (uniqueFlag) { handler.solveUniqueSolutions(); }
    if (printFlag) { handler.printAllSolutions(); }
    if (gameFlag) { handler.printAllGameBoards(); }
    if (writeFlag) { handler.writeAllSolutions(); }
    if (writeGBFlag) { handler.writeAllGameBoards(); }
    if (writeUniqueFlag) { handler.writeUniqueSolutions(); }
    if (writeUniqueGBFlag) { handler.writeUniqueGameBoards(); }
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
            printf("Number of solutions = %i \n", (2 * handler.numberOfSolutions));
        }
    }

    if (uniqueFlag) {
        printf("Number of unique solutions = %i \n",(handler.numberOfUniqueSolutions));
    }

    printf("Execution time = %f [s] \n",((double)(clock() - tStart)/CLOCKS_PER_SEC));

    return 0;
}
