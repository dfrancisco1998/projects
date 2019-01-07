#ifndef KNIGHT_H
#define KNIGHT_H

#include "Piece.h"

class Knight : public Piece
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
	  int diffN = endN - startN;
	  //we need bas value
	  if(diffN < 0){diffN *= -1;}
	  //we need abs value
	  int diffC = endC - startC;
	  if(diffC <0){diffC *= -1;}
	  
	  if(endB && startB){
	    //this just looks at the actual movement
	    //i think this is pretty succuint
	    //it deals with abs value and this is the only movemtn they can make
	    return((diffN == 2 && diffC == 1) || (diffN == 1 && diffC == 2));

	  }
	  return false;
	}
	
	
	char to_ascii( void ) const { return is_white() ? 'N' : 'n'; }

private:
 Knight( bool is_white ) : Piece( is_white ) {}

	friend Piece* create_piece( char );
};

#endif // KNIGHT_H
