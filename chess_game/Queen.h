#ifndef QUEEN_H
#define QUEEN_H

#include "Piece.h"

class Queen : public Piece
{
public:
  

	bool legal_move_shape( std::pair< char , char > start , std::pair< char , char > end ) const
	{
	  int startN = start.second - '0';
	  char startC = start.first ;
	  int endN = end.second - '0';
	  char endC = end.first;
	  bool endB;
	  bool startB;
	  //checking the board to see if the locations are legit
	  if(endN < 9 && endN >0){
	    if(endC >='A' && endC < 'I'){
	      endB = true;
	    }
	    else{endB = false;}
	  }
	  else{endB = false;}
	  
	  if(startN < 9 && startN >0){
	    if(startC >='A' && startC < 'I'){
	      startB  = true;
	    }
	    else{startB = false;}
	  }
	  else{startB = false;}
	  //we just checked to make sure the spaces are valid spaces on an empty board
	  int numD;
	  char charD;
	  //endB = true; charD = true;
	  if(endB && startB){
	    //to check if the queen can move diagonal we just need to make sure the numD and charD are the same
	    //think of it like the slope of line in which it must be equal to one
	    numD = endN - startN;
	    if(numD < 0){ numD *= -1;}
	    charD = endC - startC;
	    if( charD < 0){ charD *= -1;}
	    if(charD == numD){
	      return true;
	    }
	    else if(numD == 0 && charD < 8){
	      return true;
	    }
	    else if(numD < 8 && charD == 0){
		return true;
	    }
	    else{return false;}
	  
	  }
	  return false;
	 }

	char to_ascii( void ) const { return is_white() ? 'Q' : 'q'; }

private:
	
	Queen( bool is_white ) : Piece( is_white ) {}

	friend Piece* create_piece( char );
};

#endif // ROOK_H
