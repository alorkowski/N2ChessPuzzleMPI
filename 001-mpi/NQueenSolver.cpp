/*
 * NQueenSolver.cpp
 *
 * DESCRIPTION: Contains the backtracking algorithm and methods to filter out unique solutions.
 *
 * Created on: May 21, 2017
 *
 * AUTHORS:
 * Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
 */

#include "NQueenSolver.hpp"

#define conflict(i, j, col) (hist.getState().at(j) == i || abs(hist.getState().at(j) - i) == col - j)

void NQueenSolver::solve(int rootColumn,
                         int rootValue,
                         int numberOfQueens,
                         int col,
                         Chessboard &hist,
                         std::vector<Chessboard> &solutions,
                         int &count) {
    if (col == numberOfQueens) {
        count++;
        solutions.push_back(hist);
        return;
    }

    /* Backtracking Algorithm
     * Continue until all initial positions of the Queen
     * on the first column are explored.
     * */
    for (int k = 0, j = 0; k < numberOfQueens; k++) {
        for (j = 0; j < col && !conflict(k, j, col); j++);
        if (j < col) {
            continue;
        }
        hist.setState(col,k);
        if ( hist.getState().at(rootColumn) != rootValue ){
            return;
        }
        solve(rootColumn, rootValue, numberOfQueens, col + 1, hist, solutions, count);
    }
}

int NQueenSolver::RotaGB(int i,
                         int n,
                         Chessboard inmat) {
    int rotamat=0;
    int x = n-i-1;
    for (int k = 0; k < n; k++){
        if (x == inmat.getState().at(k)){
            rotamat += k;
        }
    }
    return rotamat;
}

int NQueenSolver::MirrorGB(int i,
                           int n,
                           Chessboard inmat) {
    int mirmat = n - 1 - inmat.getState().at(i);
    return mirmat;
}

bool NQueenSolver::CheckGB(int i,
                           int n,
                           int num,
                           Chessboard altmat,
                           std::vector<Chessboard> &mat) {
    int test;
    for (int m = 0; m < i; m++){
        test = 0;
        for (int j = 0; j < n; j++){
            if (altmat.getState().at(j) == mat.at(m).getState().at(j)){
                test++;
            }
            if (test == n - 1 ){
                return false;
            }
        }
    }
    return true;
}

void NQueenSolver::UniqGB(int i,
                          int numberOfQueens,
                          std::vector<Chessboard> &mat,
                          std::vector<Chessboard> &umat,
                          int &count) {
    Chessboard matM(numberOfQueens);
    Chessboard mat90(numberOfQueens), mat180(numberOfQueens), mat270(numberOfQueens);
    int num = (int) mat.size();

    // Extract a solution to test for uniqueness
    // 90-degree rotation.
    for (int r = 0; r < numberOfQueens ; r++){mat90.setState(r, RotaGB(r, numberOfQueens, mat.at(i)));}
    if (!CheckGB(i, numberOfQueens, num, mat90, mat)){return;}

    // 180-degree rotation.
    for (int r = 0; r < numberOfQueens ; r++){mat180.setState(r, RotaGB(r, numberOfQueens, mat90));}
    if (!CheckGB(i, numberOfQueens, num, mat180, mat)){return;}

    // 270-degree rotation.
    for (int r = 0; r < numberOfQueens ; r++){mat270.setState(r, RotaGB(r, numberOfQueens, mat180));}
    if (!CheckGB(i, numberOfQueens, num, mat270, mat)){return;}

    // Reflection of the original Game Board.
    for (int r = 0; r < numberOfQueens ; r++){matM.setState(r, MirrorGB(r, numberOfQueens, mat.at(i)));}
    if (!CheckGB(i, numberOfQueens, num, matM, mat)){return;}

    // 90-degree rotation of the Mirror Board.
    for (int r = 0; r < numberOfQueens ; r++){mat90.setState(r, RotaGB(r, numberOfQueens, matM));}
    if (!CheckGB(i, numberOfQueens, num, mat90, mat)){return;}

    // 180-degree rotation of the Mirror Board.
    for (int r = 0; r < numberOfQueens ; r++){mat180.setState(r, RotaGB(r, numberOfQueens, mat90));}
    if (!CheckGB(i, numberOfQueens, num, mat180, mat)){return;}

    // 270-degree rotation of the Mirror Board.
    for (int r = 0; r < numberOfQueens ; r++){mat270.setState(r, RotaGB(r, numberOfQueens, mat180));}
    if (!CheckGB(i, numberOfQueens, num, mat270, mat)){return;}

    /* If the solution passes all the test, then
       the solution is unique. */
    umat.push_back(mat.at(i));
    count++;
}

