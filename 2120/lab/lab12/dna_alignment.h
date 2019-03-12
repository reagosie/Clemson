/*
 * Name: Reagan Leonard
 * Date Submitted: 12/7/18
 * Lab Section: 002
 * Assignment Name: Lab12
 */

 #include <stdio.h>
 #include <iostream>
 #include <sstream>
 #include <fstream>
 #include <vector>
 #include <string>

 using namespace std;

 class DNA{
 public:
   struct Match{
     string animal = "";
     string dna = "";
     bool operator==(Match &second){
       if(this->animal == second.animal && this->dna == second.dna){
         return true;
       }
       return false;
     }
   };
     string sample;
     vector<Match> animals;
     void printPair(pair<DNA::Match, DNA::Match> bestpair);
     pair<DNA::Match, DNA::Match> process();
     double difference = 10000000;

 private:
     double calcDiff(string sample, string currentanimal);
     double distance(string sample, string currentanimal);
 };

 pair<DNA::Match, DNA::Match> DNA::process(){
   Match match;
   Match tempmatch;
   pair<Match, Match> bestpair;
   string currentline;
   double diff = 10000000;

   ifstream inputFile;
   inputFile.open("dna-samples.txt");

   if(!inputFile){
     cout << "Could not open file!";
     exit(1);
   }

   while(getline(inputFile, currentline)){
     if(currentline[0] == '-' && currentline[1] == '-'){
       animals.push_back(tempmatch);
       tempmatch.animal = currentline.substr(2, currentline.size()-3);
       tempmatch.dna = "";
     } else {
       tempmatch.dna += currentline.substr(0, currentline.size()-1);
     }
   }
   animals.push_back(tempmatch);
   animals.erase(animals.begin());

  for(auto match : animals){
    for(auto i : animals){
      if(i == match){
        continue;
      }
      diff = calcDiff(i.dna, match.dna);

      cout << i.animal << "-" << match.animal << " " << diff << endl;

      if(diff < difference){
        difference = diff;
        bestpair.first.dna = match.dna;
        bestpair.first.animal = match.animal;
        bestpair.second.dna = i.dna;
        bestpair.second.animal = i.animal;
        cout << bestpair.first.animal << " " << bestpair.second.animal << endl;
      }
    }

   }

   printPair(bestpair);
   return bestpair;
 }

 double DNA::calcDiff(string sample, string currentanimal){
   double difference = distance(sample, currentanimal);
   return difference;
 }

 double DNA::distance(string sample, string currentanimal){
   int m11 = 0; //represents the total number when A and B are the same
   int m01 = 0; //represents the total number when A and B are different

   //feed every letter of query and currentword into separate vectors
   vector<char> qstring;
   for(auto i : sample){
     qstring.push_back(i);
   }
   vector<char> wstring;
   for(auto j : currentanimal){
     wstring.push_back(j);
   }

   //loop through every character in currentword and put them in one of these 2 categories (does it match a char in query or not)
   int len = (qstring.size() < wstring.size() ? qstring.size() : wstring.size());
   for(int i = 0; i < len; i++){
     if(wstring[i] == qstring[i]){ //check same index for matching letter
       m11++;
     }
     else if(wstring[i] == qstring[i-1]){ //check letter behind (just in case it's misspelled/letter omitted)
       m11++;
     }
     else if(wstring[i] == qstring[i+1]){ //check next letter (just in case it's misspelled/letter omitted)
       m11++;
     }
     else{
       m01++;
     }
   }

   //Jaccard similarity coefficient
   double J = (double)m11/qstring.size();

   return (1-J)*10;
 }

 void DNA::printPair(pair<DNA::Match, DNA::Match> bestpair){
   cout << endl << "BEST MATCH: " << bestpair.first.animal << "-" << bestpair.second.animal << " " << difference << endl; //print bestmatch first
 }
