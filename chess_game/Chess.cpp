#include "Chess.h"
#include <vector>
using std::vector;
using std::cout;
using std::endl;


Chess::Chess( void ) : _turn_white( true )
{
	// Add the pawns
	for( int i=0 ; i<8 ; i++ )
	{
		_board.add_piece( std::pair< char , char >( 'A'+i , '1'+1 ) , 'P' );
		_board.add_piece( std::pair< char , char >( 'A'+i , '1'+6 ) , 'p' );
	}

	// Add the rooks
	_board.add_piece( std::pair< char , char >( 'A'+0 , '1'+0 ) , 'R' );
	_board.add_piece( std::pair< char , char >( 'A'+7 , '1'+0 ) , 'R' );
	_board.add_piece( std::pair< char , char >( 'A'+0 , '1'+7 ) , 'r' );
	_board.add_piece( std::pair< char , char >( 'A'+7 , '1'+7 ) , 'r' );

	// Add the knights
	_board.add_piece( std::pair< char , char >( 'A'+1 , '1'+0 ) , 'N' );
	_board.add_piece( std::pair< char , char >( 'A'+6 , '1'+0 ) , 'N' );
	_board.add_piece( std::pair< char , char >( 'A'+1 , '1'+7 ) , 'n' );
	_board.add_piece( std::pair< char , char >( 'A'+6 , '1'+7 ) , 'n' );

	// Add the bishops
	_board.add_piece( std::pair< char , char >( 'A'+2 , '1'+0 ) , 'B' );
	_board.add_piece( std::pair< char , char >( 'A'+5 , '1'+0 ) , 'B' );
	_board.add_piece( std::pair< char , char >( 'A'+2 , '1'+7 ) , 'b' );
	_board.add_piece( std::pair< char , char >( 'A'+5 , '1'+7 ) , 'b' );

	// Add the kings and queens
	_board.add_piece( std::pair< char , char >( 'A'+3 , '1'+0 ) , 'Q' );
	_board.add_piece( std::pair< char , char >( 'A'+4 , '1'+0 ) , 'K' );
	_board.add_piece( std::pair< char , char >( 'A'+3 , '1'+7 ) , 'q' );
	_board.add_piece( std::pair< char , char >( 'A'+4 , '1'+7 ) , 'k' );
}

bool Chess::checkBlock(std::pair< char , char > start , std::pair< char , char > end ){
  //cout<< "in"<< endl;
  if(in_check(_turn_white)){
    char tmped = _board(end)->to_ascii();
    char tmpst = _board(start)->to_ascii();
    _board .eraseAt(start);
    _board .eraseAt(end);
    cout<<" before" << endl;
    if(tmpst == 'P' && end.second == '8'){
      //cout<<"in here"<< endl;
      _board.addAt(end, 'Q');
    }
    else if(tmpst == 'p' && end.second == '1'){
      _board.addAt(end, 'q');
    }
    else{
      _board.addAt(end, tmpst);
    }
    if(in_check(_turn_white))	{
      _board .eraseAt(end);
      _board.addAt(start, tmpst);
      _board.addAt(end, tmped);
      cout<<"Did not evade check"<<endl;
      return false;
    }
    _turn_white = !_turn_white;
    return true;
  }else{
    char tmp = _board(start)->to_ascii();
    _board .eraseAt(start);
    _board .eraseAt(end);
    if(tmp == 'P' && end.second == '8'){
      //cout<<"in here"<< endl;
      _board.addAt(end, 'Q');
    }
    else if(tmp == 'p' && end.second == '1'){
      _board.addAt(end, 'q');
    }
    else{
      _board.addAt(end, tmp);
    }    
    _turn_white = !_turn_white;
    return true;
  }
}

bool Chess::make_move(std::pair< char , char > start , std::pair< char , char > end ){
  if(!(_board(start)!=NULL)){
    cout << "Invalid move" << endl;
    return false;
  } 
  const Piece* pceStrt = _board(start);
  if(pceStrt->is_white() != _turn_white){
    cout << "Invalid move" << endl;
    return false; // the first peice is not the player whose turn it is
  }  
  if(in_mate(_turn_white)){
    cout<<"Youre in checkmate"<<endl;
    return false;
  }  
  if(in_stalemate(_turn_white)){
    cout<<"Youre in stalemate"<<endl;
    return false;
  }
  //Must distinguish if it is a capture or a move
  if(_board(end)!=NULL){
    //this means it is a capture move
    const Piece* pceEnd = _board(end);
    if(pceEnd->is_white() == pceStrt->is_white()){//we know it is the same color at start and end
      cout << "Invalid move" << endl;
      return false;
    } else {//we know diffrent colors
      if(pceStrt->legal_capture_shape(start, end)){
	if  (pceStrt->to_ascii() != 'N' || pceStrt->to_ascii() != 'n'){
	  if(_board.path_is_clear(start, end)){
	    
	    return checkBlock(start, end);
	  }
	  else {
	    cout << "Invalid move" << endl;
	    return false;
	  }
	}else {
	  return checkBlock(start, end);
	}	
      }      
    }
    return false;
  }
  //If we get to this point, we know that it is not a caputre move
  if(pceStrt->legal_move_shape(start, end)) {
    if (pceStrt->to_ascii() != 'N' || pceStrt->to_ascii() != 'n'){
      if(_board.path_is_clear(start, end)){
	return checkBlock(start, end);
      }
      else {
	cout << "Invalid move" << endl;
	return false;
	
      }
    }
    else {
      char tmp = _board(start)->to_ascii();
      _board .eraseAt(start);
      _board .eraseAt(end);
      _board.addAt(end, tmp);
      _turn_white = !_turn_white;
      return true;
    }
    cout << "Invalid move" << endl;
    return false;
  }  
  //We know its not legal if we get here
  cout<<"Illegal Move"<<endl;
  return false;
}
  
  








std::pair <char,char> findPeice (Board b, char needle){
  for(char i = '1'; i <= '8'; i++){
    for(char j = 'A'; j <= 'H'; j++){
      std::pair <char,char> pos = std::make_pair (j,i);
      if(b(pos) != NULL && b(pos)->to_ascii() == needle){
				return pos;
      }
    }
  }
  return std::make_pair ('-','-'); //piece was not found
}

bool Chess::peiceInCheck(bool w, std::pair< char , char > testLoc) const{
	for(char i = '1'; i <= '8'; i++){
		for(char j = 'A'; j <= 'H'; j++){
			std::pair <char,char> pos = std::make_pair (j,i);
			if(_board(pos) != NULL){
				//NOW TEST TO SEE IF OPPOSITE colors
				bool correctColor = false;
				if(_board(pos)->to_ascii() >= 'A' && _board(pos)->to_ascii() <= 'Z'){
				if(!w){
						correctColor = true;
					}
				} else {
					if(w){
						correctColor = true;
					}
				}
				if(correctColor && _board(pos)->legal_capture_shape(pos, testLoc)){
					return true;
				}
			}
		}
	}
	return false;
}

bool Chess::in_check( bool white ) const {
	std::pair <char,char> kng;
	if(white){//IS WHITE UPPER CASE
		kng = findPeice(_board, 'K');
	} else {
		kng = findPeice(_board, 'k');
	}
  return this->peiceInCheck(white, kng);
}

bool Chess::in_mate( bool white ) const {
	if(!this->in_check(white)){
		return false;
	}
	std::pair <char,char> kng;
	if(white){//IS WHITE UPPER CASE
		kng = findPeice(_board, 'K');
	} else {
		kng = findPeice(_board, 'k');
	}
	vector<std::pair<char, char>> vec;
	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			char fst = kng.first + i;
			char snd = kng.second + j;
			if(!((fst) < 'A' || (fst) > 'H')){
				if(!((snd) < '1' || (snd) > '8')){
					if(_board(std::make_pair(fst, snd)) == NULL){
						vec.push_back(std::make_pair(fst, snd));
					}
				}
			}
		}
	}

	for (vector<std::pair<char, char>>::iterator it = vec.begin() ; it != vec.end(); ++it)
    if(!peiceInCheck(white, *it)){
			return false;
		}

		return true;
}

bool Chess::in_stalemate( bool white ) const {
	for(char i = '1'; i <= '8'; i++){
    for(char j = 'A'; j <= 'H'; j++){
			if(_board(std::make_pair(j,i))!=NULL){
			char c = _board(std::make_pair(j,i))->to_ascii();
			if(c>'A' && c < 'Z'){//we know the piece is white
				if(white){
					if(c != 'K'){
						return false;
					}
				}
			} else {
				if(!white){
					if(c!='k'){
						return false;
					}
				}
			}
		}
		}
	}
	std::pair <char,char> kng;
	if(white){//IS WHITE UPPER CASE
		kng = findPeice(_board, 'K');
	} else {
		kng = findPeice(_board, 'k');
	}
	vector<std::pair<char, char>> vec;
	for(int i = -1; i <= 1; i++){
		for(int j = -1; j <= 1; j++){
			if(i!=0 && j!=0){}
			char fst = kng.first + i;
			char snd = kng.second + j;
			if(!((fst) < 'A' || (fst) > 'H')){
				if(!((snd) < '1' || (snd) > '8')){
					if(_board(std::make_pair(fst, snd)) == NULL){
						vec.push_back(std::make_pair(fst, snd));
					}
				}
			}
		}
	}


	for (vector<std::pair<char, char>>::iterator it = vec.begin() ; it != vec.end(); ++it){
    if(!peiceInCheck(white, *it)){
			return false;
		}
	}

		return true;
}


std::ostream& operator << ( std::ostream& os , const Chess& chess )
{
	// Write the board out and then either the character 'w' or the character 'b', depending on whose turn it is
	return os << chess.board() << ( chess.turn_white() ? 'w' : 'b' );
}

std::istream& operator >> ( std::istream& is , Chess& chess )
{

	char c;
	for(char i = '8'; i >= '1'; i--){
		for(char j = 'A'; j <= 'H'; j++){
			is >> c;
			chess._board.eraseAt(std::make_pair (j,i));
			if(c != '-'){
				chess._board.add_piece(std::make_pair (j,i), c);
			}
		}
	}
	is >> c;
	if(c == 'w'){
		chess._turn_white = true;
	} else {
		chess._turn_white = false;
	}
	return is;
}
