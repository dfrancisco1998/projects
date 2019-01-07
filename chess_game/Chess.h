#ifndef CHESS_H
#define CHESS_H

#include <iostream>
#include "Piece.h"
#include "Board.h"

class Chess
{
public:
	// This default constructor initializes a board with the standard piece positions, and sets the state to white's turn
	Chess( void );

	// Returns a constant reference to the board
	
	const Board& board( void ) const { return _board; }

	// Returns true if it's white's turn
	bool turn_white( void ) const { return _turn_white; }

	// Attemps to make a move. If successfull, the move is made and the turn is switched white <-> black
	bool make_move( std::pair< char , char > start , std::pair< char , char > end );

	bool checkBlock(std::pair< char , char > start , std::pair< char , char > end );

	// Returns true if the designated player is in check
	bool in_check( bool white ) const;

	// Returns true if the designated player is in mate
	bool in_mate( bool white ) const;

	// Returns true if the designated player is in mate
	bool in_stalemate( bool white ) const;

	friend std::istream& operator >> ( std::istream& is , Chess& chess );

private:
	//ADDED THESE PRIVATE HELPER FUNCTIONS
	bool peiceInCheck(bool w, std::pair< char , char > testLoc) const;

	// The board
	Board _board;

	// Is it white's turn?
	bool _turn_white;
};

// Writes the board out to a stream
std::ostream& operator << ( std::ostream& os , const Chess& chess );

// Reads the board in from a stream
std::istream& operator >> ( std::istream& is , Chess& chess );


#endif // CHESS_H
