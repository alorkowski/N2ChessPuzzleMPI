//
// Created by Alexander Lorkowski on 5/2/17.
//

#ifndef NQUEENSOLVER_HPP_
#define NQUEENSOLVER_HPP_

#include <cstdlib>
#include <vector>
#include "Chessboard.hpp"

class NQueenSolver {
public:

    virtual void solve(int rootColumn,
                       int rootValue,
                       int numberOfQueens,
                       int col,
                       Chessboard &hist,
                       std::vector<Chessboard> &solutions,
                       int &count);

    int RotaGB(int i,
               int n,
               Chessboard inmat);

    int MirrorGB(int i,
                 int n,
                 Chessboard inmat);

    bool CheckGB(int i,
                 int n,
                 int num,
                 Chessboard altmat,
                 std::vector<Chessboard> &mat);

    void UniqGB(int i,
                int numberOfQueens,
                std::vector<Chessboard> &mat,
                std::vector<Chessboard> &umat,
                int &count);

private:
    //bool conflict(Chessboard &hist, int col, int i, int j);
};

#endif //NQUEENSOLVER_HPP_
