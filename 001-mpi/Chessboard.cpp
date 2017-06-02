/*
 * Chessboard.cpp
 *
 * DESCRIPTION: A class used to create an object storing a possible solution to the NQueen problem.
 *
 * Created on: May 21, 2017
 *
 * AUTHORS:
 * Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
 */

#include "Chessboard.hpp"
#include <algorithm>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cassert>
#include <cmath>
#include <iostream>

Chessboard :: Chessboard(int size) {
    std::vector<int> state(size, 0);
    this->state = state;
}

Chessboard::~Chessboard() {}

size_t Chessboard::getSize() const {
	return state.size();
}

void Chessboard::setState(int index, int position) {
    state[index] = position;
}

std::vector<int> Chessboard::getState() {
	return state;
}

void Chessboard::print() {
    for (int i = 0; i < getSize(); i++) {
        std::cout << getState().at(i) << ' ';
    }
    std::cout << std::endl;
}

void Chessboard::printGameBoard() {
    for (int i = 0; i < getSize(); i++){
        for (int j = 0; j < getSize(); j++){
            if (j == state.at(i)) {
                std::cout << "Q";
            } else {
                std::cout << "#";
            }
        }
        std::cout << std::endl;
    }
}
