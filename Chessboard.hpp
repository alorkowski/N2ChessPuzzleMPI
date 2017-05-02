#ifndef CHESSBOARD_HPP_
#define CHESSBOARD_HPP_

#include <vector>
#include <stdint.h>
#include <string>

class Chessboard {
public:
	// Constructor for a chessboard of with (non-zero) sides of length size.
	// Size represents the 'N' parameter in the N-Queens problem.
	Chessboard(int size);

	// Destructor.
	~Chessboard();

	// Returns the size of the chessboard.
	size_t getSize() const;

    // Return the raw board representation. This is used to set the board.
    void setState(int index, int position);

	// Return the raw board representation. This is used to set the board.
	std::vector<int> getState();

    // vector representing the board.
    std::vector<int> state;

	void print();

	void printGameBoard();

private:
};

#endif	//CHESSBOARD_HPP_
