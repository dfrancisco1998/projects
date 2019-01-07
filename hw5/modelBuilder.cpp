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
string makeLower(string word){
  for(int i = 0; i < (int)word.length(); i++){
    word[i] = tolower(word[i]);
  }
  return word;
}


map<pair<string, string>, int> makeWordFreqMap(vector<string> text, map<pair<string, string>, int> wordFreq){
  pair<string,string> wordP;
  for (vector<string>::const_iterator it = text.cbegin(); it != text.cend()-1; ++it) {
    wordP =  make_pair(*it, *(it+1));
    wordFreq[wordP]++;
  }
  return wordFreq;
}

void printOut(string sPatt, map<pair<string, string>, int> wordFreq){
  if(sPatt=="a"){
    for (map<pair<string, string>, int>::const_iterator that = wordFreq.cbegin(); that != wordFreq.cend(); ++that) {
      cout << that->first.first << " " << that->first.second << " " << that->second <<endl;
    }
    //break;
  }
  else if(sPatt == "r"){
    for (map<pair<string, string>, int>::const_reverse_iterator that = wordFreq.crbegin(); that != wordFreq.crend(); ++that) {
      cout << that->first.first << " " << that->first.second << " " << that->second <<endl;
    }
    // break;
  }
  
  else if(sPatt == "c"){
    map<int, vector<pair<string, string>> > words_by_freq;
    for (map<pair<string,string>, int>::const_iterator the  = wordFreq.cbegin(); the != wordFreq.cend(); ++the) {
      if(words_by_freq.find(the->second) != words_by_freq.end()){
        words_by_freq.at(the->second).push_back(the->first);
      }
      else{
        //words_by_freq[the->second] = vector<pair<string, string>>;
        words_by_freq[the->second].push_back(the->first);
	
      }
    }
    for (map<int, vector<pair<string, string>>>::const_reverse_iterator thiis = words_by_freq.crbegin(); thiis != words_by_freq.crend(); ++thiis) {
      vector<pair<string, string>> temp = thiis->second;
      for(vector<pair<string, string>> :: const_iterator it = temp.cbegin(); it != temp.cend(); it++){
        cout <<it->first << " " << it->second << " " << thiis->first<<endl;
      }
    }
     
  }
  
  else{
    cout<<"that sort is not possible"<<endl;
  }
}

void printViaSeed(string seed, map<pair<string, string>, int> wordFreq){
  
  int max = 0;
  int counter = 0;
  string maxS;
  string tempSeed;
  while(counter <15 && seed.compare("end") !=0){
    max = 0;
    for(map<pair<string, string>, int>::const_iterator it = wordFreq.cbegin(); it != wordFreq.cend(); it++){
      string firstW  = (it->first).first;
      string secondW = (it->first).second;
      int freq = it->second;
      if(firstW.compare(seed) == 0){
        if(freq > max){
          max = freq;
          maxS = secondW;
        }
      }
    }
    if(max == 0){
      cout<< "end";
    }
    cout<<seed + " " ;
    seed = maxS;
    counter++;
  }
  cout<<endl;
}
