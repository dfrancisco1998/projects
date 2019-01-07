#ifndef KING_H
#define KING_H

#include "Piece.h"

class King : public Piece
{
public:
  

	bool legal_move_shape( std::pair< char , char > start , std::pair< char , char > end ) const
	{
	  int startN = start.second - '0';;
	  char startC = start.first;
	  int endN = end.second - '0';;
	  char endC =  end.first;
	  bool endB;
	  bool startB;
	  //this tests to make sure the inouts rae within bounds
	  if(endN < 9 && endN >0){
	    if(endC >='A' && endC < 'I'){
	      endB = true;
	    }
	    else{endB = false;}
	  }
	  else{endB = false;}
	  //this checks to see if the start are in bounds
	  if(startN < 9 && startN >0){
	    if(startC >='A' && startC < 'I'){
	      startB  = true;
	    }
	    else{startB = false;}
	  }
	  else{startB = false;}
	  //this says if the bounds are ok then we can do the actual move checking
	  if(endB && startB){
	    //numdiff descibes the actual size of the movement across the numbers
	    int numDiff = endN - startN;
	    if(numDiff < 0){ numDiff *= -1;}
	    //this describes the actual size of the movement across the letters
	    char charDiff = endC - startC;
	    if(charDiff < 0){ charDiff *= -1;}
	    //this is checking diagnoal movements
	    if(numDiff == 1 &&  charDiff== 1){return true;}
	    //just checking side to side movement
	    else if(numDiff == 0){
	      if(charDiff == 1){
		return true;
	      }
	      else{return false;}
	    }
	    //this is checking up and down move
	    else if(numDiff == 1){
	      if(charDiff == 0){
		return true;
	      }else{return false;}
	    }
	    
	    else{return false;}
	    
	  }
	  return false;
	}

	char to_ascii( void ) const { return is_white() ? 'K' : 'k'; }
	
 private:

 King( bool is_white ) : Piece( is_white ) {}
	
	friend Piece* create_piece( char );
};

#endif // KING_H
