//! Chessboard.hpp
/*!
  \brief A class constructing an object storing the state of the NQueen problem.
  \author Lorkowski, Alexander <alexander.lorkowski@epfl.ch>
  \version 1.0
  \date  21 May 2017
  \remark Ecole Polytechnic Federal de Lausanne (EPFL)
  \remark MATH-454 Parallel and High-Performance Computing
*/

#ifndef CHESSBOARD_HPP_
#define CHESSBOARD_HPP_

#include <vector>
#include <stdint.h>
#include <string>
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <cassert>
#include <cmath>
#include <iostream>

class Chessboard {
public:
	/*! A constructor to instantiate variables for Chessboard object.
     *
     * \param size The total number of queens and consequently the length of the chessboard.
     */
	Chessboard(int size);

	/*! A virtual destructor for the Chessboard object.
	 */
	~Chessboard();

	/*! A method when called returns the size of the Chessboard object.
     *
     * \return The size of the Chessboard object.
     */
	size_t getSize() const;

	/*! A method to set the position of a queen on the chessboard.
     *
     * \param index The row where to place the queen.
     * \param position The column where to place the queen.
     */
    void setState(int index, int position);

	/*! A method to get the current state of the Chessboard object.
     *
     * \return The state of the Chessboard object in a vector.
     */
	std::vector<int> getState();

	/*! A method to print the current state of the Chessboard object.
     */
	void print();

    /*! A method to print the current state of the Chessboard object in a traditional chessboard format.
     */
	void printGameBoard();

private:
    /*! A vector containing the current state of the Chessboard object.
     */
	std::vector<int> state;
};

#endif	//CHESSBOARD_HPP_
