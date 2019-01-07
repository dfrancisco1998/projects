#ifndef MODELBUILDER_H
#define MODELBUILDER_H 
#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<algorithm>
#include<utility>
#include<cctype>


//this is to make the model case insensitive

std::string makeLower(std::string word);
//this is to make the model and hte most necessary vector that organizes the pairs and their frequency 
std::map<std::pair<std::string, std::string>, int> makeWordFreqMap(std::vector<std::string> text, std::map<std::pair<std::string, std::string>, int> wordFreq);
//this prints out the specific patterns 
void printOut(std::string sPatt, std::map<std::pair<std::string, std::string>, int> wordFreq);
//this prints the sentence out
void printViaSeed(std::string seed, std::map<std::pair<std::string, std::string>, int> wordFreq);

#endif
