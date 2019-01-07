#ifndef PAWN_H
#define PAWN_H

#include "Piece.h"

class Pawn : public Piece
{
public:


	bool legal_move_shape( std::pair< char , char > start , std::pair< char , char > end ) const
	{
	  char startC = start.first;
	  int startN = start.second - '0';
	  char endC = end.first;
	  int endN = end.second - '0';

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
	  //now we have to check if the pawn movement is ok
	  //pawn can only move foward and not backward
	  //so the letter will be held constant aka == 0
	  int numD = endN - startN;
	  int charD = endC - startC;
	  if(!(this->is_white())){
	    numD *= -1;
	    //charD *= -1;

	  }
	  //endB = true; startB = true;
	  if(endB && startB){
	    //this is taking care of
	    if(charD == 0){
	      if(this->is_white() && start.second == '2'){
		  if(numD == 2 || numD == 1){
		    return true;
		  }
		  else{return false;}
	      }
	      else if((!this->is_white()) && start.second =='7') {

		if(numD == 2 || numD == 1){
		  return true;
		}
	      }
	      else{
		if(numD == 1){
		  return true;
		}
		return false;
	      }
	    }
	  }
	  return false;

	}
	bool legal_capture_shape( std::pair< char , char > start , std::pair< char , char > end ) const{
	  // if(this->legal_move_shape(start,end)){
	    char startC = start.first;
	    int startN = start.second - '0';
	    char endC = end.first;
	    int endN = end.second - '0';
	    int numD = endN - startN;
	    int charD = endC - startC;
	    if(!this->is_white()){
	      numD *= -1;
	      charD *= -1;
	    }
	    if(numD == 1 && charD == 1){
	      return true;
	    }

	    //}
	  return false;
	}
	
	
	char to_ascii( void ) const { return is_white() ? 'P' : 'p'; }

 private:
	
 Pawn( bool is_white ) : Piece( is_white ) {}

	friend Piece* create_piece( char );
};

#endif // PAWN_H
