#include <iostream>
#include <utility>
#include <map>
#ifndef _WIN32
#include "Terminal.h"
#endif // !_WIN32
#include "Board.h"
#include "CreatePiece.h"

using std::string;
using std::cout;
using std::endl;
using std::map;
using std::pair;

Board::Board( void ){}

Board::~Board(){
  //cout<<"TESTA1"<<endl;
  for(char i = '1'; i <= '8'; i++){
		for(char j = 'A'; j <= 'H'; j++){
      //std::pair<char, char> foo = std::make_pair (j,i);
			//cout<<foo.first<< foo.second<<endl;
      if(_occ.count(std::make_pair (j,i))){
        Piece * tmp = _occ.at(std::make_pair (j,i));
        _occ.erase(std::make_pair (j,i));
        delete tmp;
      }
   }
 }
}
bool Board::eraseAt(std::pair< char , char > position){
  if(_occ.count(position) != 0){
    Piece* tmp = _occ.at(position);
    _occ.erase(position);
    delete tmp;
    return true;
  }
  return false;
}

bool Board::addAt(std::pair< char , char > position, char pce){
  if(_occ.count(position) == 0){
    _occ[position] = create_piece(pce);
    return true;
  }
  return false;
}



Board::Board(const Board& o){

  for(map< pair< char , char > , Piece* >::const_iterator it = o._occ.cbegin(); it != o._occ.cend(); it++){
    add_piece(it->first, it->second->to_ascii());
  }
  //delete o;
}

const Piece* Board::operator()( std::pair< char , char > position ) const
{
  if(_occ.count(position) == 0){
    return NULL;
  }
  return _occ.at(position);
}



bool Board::add_piece( std::pair< char , char > position , char piece_designator )
{
  Piece* p = create_piece(piece_designator);
//cout<<"TESTB1"<<endl;
  //cout <<position.first<<" "<<position.second<<endl;

  if(_occ.count(position) != 0){
    //cout<<"TESTB2"<<endl;
    return false;
  }
  _occ[position] = p;
  return true;
}

bool Board::has_valid_kings( void ) const
{
  int c = 0;
  for (map<std::pair< char , char > , Piece* >::const_iterator it=_occ.cbegin(); it!=_occ.cend(); ++it){

    //cout<<it->second->to_ascii()<<endl;
    if(it->second->to_ascii() == 'K' || it->second->to_ascii() == 'k'){
      c++;
    }
  }
  if(c==2){
    return true;
  }

	return false;
}

void Board::display( void ) const
{
  const string header("        Hopkins Chess         ");
  const string col_names("ABCDEFGH");
  const string row_names("12345678");
  Terminal term = Terminal();

  //TITLE
  term.color_all(true, term.WHITE, term.BLUE);
  cout << header;
  term.set_default();
  cout << endl;

  //Make first row
  term.color_all(true, term.WHITE, term.BLUE);
  cout << "   ";
  for(string::const_iterator it = col_names.begin();
      it != col_names.end(); ++it) {
    cout << " " << *it << " ";
  }
  cout << "   ";
  term.set_default();
  cout << endl;

  //HEADER LINE
  term.color_all(true, term.WHITE, term.BLUE);
  cout << "   ";
  for(string::const_iterator it = col_names.begin();
      it != col_names.end(); ++it) {
    cout << "---";
  }
  cout << "   ";
  term.set_default();
  cout << endl;

  //MIDDLE BIT
  //loop through rows
  for(string::const_reverse_iterator row_it = row_names.crbegin();
      row_it != row_names.crend(); ++row_it) {

    //output row header on left side of this row
    term.color_all(true, term.WHITE, term.BLUE);
    cout << *row_it << " |";
    term.set_default();

    //loop through columns
    for(string::const_iterator col_it = col_names.cbegin();
  col_it != col_names.cend(); ++col_it) {

      //output the piece type, if one exists, or a blank

      std::pair<char, char> position = std::make_pair(*col_it, *row_it);


      //output the piece type if one exists, or a blank if otherwise
      cout << " ";
      if (_occ.find(position) != _occ.end()) {
  Piece* piece = _occ.at(position);
  cout << piece->to_ascii();

      } else {
  cout << " ";
      }
      cout << " ";
    }

    //end the current row
    term.color_all(true, term.WHITE, term.BLUE);
    cout << "   ";
    term.set_default();
    cout << endl;

  }
  //LAST ROW
  term.color_all(true, term.WHITE, term.BLUE);
  cout << "   ";
  for(string::const_iterator it = col_names.begin();
      it != col_names.end(); ++it) {
    cout << "---";
  }
  cout << "   ";
  term.set_default();
  cout << endl;


}


std::ostream& operator << ( std::ostream& os , const Board& board )
{
	for( char r='8' ; r>='1' ; r-- )
	{
		for( char c='A' ; c<='H' ; c++ )
		{
			const Piece* piece = board( std::pair< char , char >( c , r ) );
			if( piece ) os << piece->to_ascii();
			else        os << '-';
		}
		os << std::endl;
	}
	return os;
}


bool Board::path_is_clear(std::pair<char, char> start, std::pair<char, char> end) const {
  /* First check to see if path is hor, vert, or diag
     Then check for direction
     Finally check for pieces in the way
  */


  //VERTICLE
  if(start.first - end.first == 0 && start.second - end.second != 0) {

    //DOWN
    if(start.second > end.second) {
      for(char i = start.second - 1; i > end.second; i--) {
	pair<char, char> current = std::make_pair(start.first, i);
	if(_occ.find(current) != _occ.end()) {
	  return false;
	}
      }
    }

    //UP
    if(start.second < end.second) {
      for(char i = start.second + 1; i < end.second; i++) {
	pair<char, char> current = std::make_pair(start.first, i);
	if(_occ.find(current) != _occ.end()) {
	  return false;
	}
      }
    }
  }

  //HORIZONTAL
  else if(start.first - end.first != 0 && start.second - end.second == 0) {

    //LEFT
    if(start.first > end.first) {
      for(char i = start.first - 1; i > end.first; i--) {
	pair<char, char> current = std::make_pair(i, start.second);
	if(_occ.find(current) != _occ.end()) {
	  return false;
	}
      }
    }

    //RIGHT
    if(start.first < end.first) {
      for(char i = start.first + 1; i < end.first; i++) {
	pair<char, char> current = std::make_pair(i, start.second);
	if(_occ.find(current) != _occ.end()) {
	  return false;
	}
      }
    }
  }

  //DIAGONAL
  else if(abs(start.first - end.first) == abs(start.second - end.second)) {

    //UP
    if(start.second < end.second) {

      //UP-RIGHT
      if(start.first < end.first) {
	for(char i = start.second + 1; i < end.second; i++) {
	  for(char j = start.first + 1; j < end.first; j++) {
	    pair<char, char> current = std::make_pair(j, i);
	    if(_occ.find(current) != _occ.end()) {
	      return false;
	    }
	  }
	}
      }

      //UP-LEFT
      else if(start.first > end.first) {
	for(char i = start.second + 1; i < end.second; i++) {
	  for(char j = start.first - 1; j > end.first; j--) {
	    pair<char, char> current = std::make_pair(j, i);
	    if(_occ.find(current) != _occ.end()) {
	      return false;
	    }
	  }
	}
      }
    }

    //DOWN
    else if(start.second > end.second) {

      //DOWN-RIGHT
      if(start.first < end.first) {
	for(char i = start.second - 1; i > end.second; i--) {
	  for(char j = start.first + 1; j < end.first; j++) {
	    pair<char, char> current = std::make_pair(j, i);
	    if(_occ.find(current) != _occ.end()) {
	      return false;
	    }
	  }
	}
      }

      //DOWN-LEFT
      else if(start.first < end.first) {
	for(char i = start.second - 1; i > end.second; i--) {
	  for(char j = start.first - 1; j > end.first; j--) {
	    pair<char, char> current = std::make_pair(j, i);
	    if(_occ.find(current) != _occ.end()) {
	      return false;
	    }
	  }
	}
      }
    }

  }
  return true;

}
