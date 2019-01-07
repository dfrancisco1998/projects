#ifndef PIECE_H
#define PIECE_H

#include <utility>

class Piece
{
public:
	// The destructor for Piece is declared virtual so derived classes can override it
	virtual ~Piece( void ){}

	// Returns information about the color of the piece.
	bool is_white( void ) const { return _is_white; }

	// Indicates whether the given start and end coordinates describe a move that would be considered valid for this piece
	// if there is no piece at the end positition (i.e. this is not a capture move)
	virtual bool legal_move_shape( std::pair< char , char > start , std::pair< char , char > end ) const = 0;

	// Indicates whether the given start and end coordinates describe a move that would be considered valid for this piece
	// if there is a piece at the end position (i.e. this is a capture move)
	virtual bool legal_capture_shape( std::pair< char , char > start , std::pair< char , char > end ) const
	{
		return legal_move_shape( start , end );
	}

	// Returns a character representing the piece type. Should be upper-case  for white and lower-case for black.
	// Characters are:
	//		King:		K/k
	//		Queen:		Q/q
	//		Bishop:		B/b
	//		Knight:		N/n
	//		Rook:		R/r
	//		Pawn:		P/p
	//		Mystery:	M/m
	virtual char to_ascii( void ) const = 0;

protected:
	// When a piece is created, it's color must be provided as an argument
	Piece( bool is_white ) : _is_white( is_white ){ }

private:
	// A boolean value indicating whether the piece is white or black
	bool _is_white;
};

#endif // PIECE_H
