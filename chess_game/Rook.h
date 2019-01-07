#ifndef ROOK_H
#define ROOK_H

#include "Piece.h"

class Rook : public Piece
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
      if(endB && startB){
	numD = endN - startN;
	charD = endC - startC;
	//moving side to side
	if(numD == 0 && charD < 8){
	  return true;
	}
	else{
	  if(numD < 8 && charD == 0){
	    return true;
	  }
	}
      }
      return false;
      
    }
  
  

  char to_ascii( void ) const { return is_white() ? 'R' : 'r'; }
  
 private:

 Rook( bool is_white ) : Piece( is_white ) {}
  
  friend Piece* create_piece( char );
};

#endif // ROOK_H
