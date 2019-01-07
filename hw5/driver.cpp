#include"modelBuilder.h"
#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
#include<cctype>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::map;
using std::vector;
using std::sort;
using std::pair;

int main(int argc, char **argv){
  //cout<<"please give a sentence. ctrl D:"<<endl;
  string seed;
  string sPatt;
  if(argc > 1 && argc == 3){
    seed = argv[1];
    sPatt = argv[2];
  }
  else{
    cout<<"you did not give enough args"<<endl;
  }

  string word;//look at each word
  vector<string> text;// then store each word
  while(cin>>word){
    //so while there is text to read we want to look at it
    for(int i = 0; i < (int)word.length(); i++){
      word[i] = tolower(word[i]);
    }
    text.push_back(word);
  }
  text.push_back("end");
  //now for the maps
  map<pair<string, string>, int> wordFreq;

  pair<string,string> wordP;
  //this isw for the c sorting
  //this isw for the c sorting
  wordFreq =  makeWordFreqMap(text, wordFreq);
  //doa switch basews a r c adn then ues the cool prints wiht rbegin
  //c is like excer
  //mpa of int to vecto of pairs
  printOut(sPatt, wordFreq);
  //now for using the seed word

  printViaSeed(seed, wordFreq);

 return 0;
}
