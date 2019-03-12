/*
 * Name: Reagan Leonard
 * Date Submitted: 11/30/18
 * Lab Section: 002
 * Assignment Name: Lab11
 */

#include <stdio.h>
#include <dirent.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

class Searcher{
public:
  struct Match{
    string file;
    string text;
    int diff = 10000000;
  };
    string query;
    vector<string> filenames;
    void printMatch(Match match);
    Match search(string query);

private:
    int calcDiff(string query, string currentword);
    double jaccard(string query, string currentword);
    vector<string> parsePhrase(string query);
    int calcWordDiff(vector<string> queryWords, string restofline);
    int charDiff = 3;
    int wordDiff = 2;
    int perfDiff = 0;
};

Searcher::Match Searcher::search(string query){
  Match match;
  Match bestmatch;
  bool ismatch = false;

  //open content directory and push file names back to vector
  DIR *dp;
  struct dirent *ep;

  dp = opendir("content");
  if (dp != NULL){
    while ((ep = readdir(dp)) != NULL){
      //instead of printing (listing out) all the files, we want to add them to this vector
      filenames.push_back(ep->d_name);
    }
    closedir(dp);
  }
  else{
    perror("Couldn't open that directory!");
  }

  // cout << calcDiff("There", "there'll") << endl;
  // cout << calcDiff("There", "There's") << endl;
  //search through filenames vector for a match
  for(auto i : filenames){
    ifstream file;
    if (i == "." || i == "..")
      continue;

    //FOR FILENAMES
    if(calcDiff(query, i) <= charDiff || calcWordDiff(parsePhrase(query), i) <= wordDiff){ //if difference between query and filename is less than or equal to 3, count as match
      if((calcDiff(query, i) == perfDiff || calcDiff(query, i) < bestmatch.diff) || calcWordDiff(parsePhrase(query), i) == perfDiff || calcWordDiff(parsePhrase(query), i) < bestmatch.diff){
        bestmatch.file = i;
        bestmatch.diff = calcDiff(query, i);
        ismatch = true;
      }
    }

    file.open("content/" + i); //open each file

    if (!file) {
      cerr << "Open file error!" << endl;
      exit(1);
    }

    //perform exhaustive search on that entire file
    string currentline;
    while(getline(file, currentline)){

      //FOR PHRASES THAT ARE THE ENTIRE LINE
      if(calcDiff(query, currentline) <= charDiff){ //if difference between query and current line is less than or equal to 3, count as match
        if(calcDiff(query, currentline) == perfDiff || calcDiff(query, currentline) < bestmatch.diff){
          bestmatch.file = i;
          bestmatch.text = currentline;
          bestmatch.diff = calcDiff(query, currentline);
          ismatch = true;
        }
      }
      else{
        string currentword;
        string restofline;
        vector<string> vector = parsePhrase(query);

        for(uint j = 0; j < vector.size(); j++){
          stringstream ss(currentline);
          stringstream s1(currentline);

          while(ss >> currentword){ //this is to check the string array of the current line to see if our query is spread out inside the line
            if(currentword != vector.at(j) || currentword == " "){
              //skip currentword (until we get to the first word of our array) and skip whitespace
            }
            else{
              restofline = currentline.substr(currentline.find(vector.at(j)), currentline.length());
              //FOR PHRASES THAT MAY NOT BE THE ENTIRE LINE, OR SPREAD OUT IN THE LINE
              if(calcWordDiff(vector, restofline) <= wordDiff){ //if difference between query and rest of line is less than or equal to 3, count as match
                if(calcWordDiff(vector, restofline) == perfDiff || calcWordDiff(vector, restofline) < bestmatch.diff){
                  bestmatch.file = i;
                  bestmatch.text = currentline;
                  bestmatch.diff = calcWordDiff(vector, restofline);
                  ismatch = true;
                }
              }
            }
          }
          for(uint k = 0; k < query.size(); k++){
            while(s1 >> currentword){
              if(currentword != " "){ //skip whitespace
                //FOR INDIVIDUAL WORDS
                if(calcDiff(query, currentword) <= charDiff){ //if difference between query and current word in line is less than or equal to 3, count as match
                  if(calcDiff(query, currentword) == perfDiff || calcDiff(query, currentword) < bestmatch.diff){
                    bestmatch.file = i;
                    bestmatch.text = currentline;
                    bestmatch.diff = calcDiff(query, currentword);
                    ismatch = true;
                  }
                }
              }
            }
          }
        }
      }
    }

    if(ismatch == true){
      match = bestmatch;
    }

    file.close();
  }

  return match;
}

int Searcher::calcDiff(string query, string currentword){
  int difference = 100000;

  if(currentword == " "){
    //skip whitespace
  }
  else if(query == currentword){
    difference = perfDiff;
  }
  else if(jaccard(query, currentword) <= 1){ //if we have to insert, delete, or subsitute 1 character
    difference = 1;
  }
  else if(jaccard(query, currentword) <= 2){ //if we have to insert, delete, or subsitute 2 characters
    difference = 2;
  }
  else if(jaccard(query, currentword) <= 3){ //if we have to insert, delete, or subsitute 2 characters
    difference = 3;
  }
  else{
    difference = 4; //4 means it is not a match
  }
  return difference;
}

double Searcher::jaccard(string query, string currentword){
  int m11 = 0; //represents the total number when A and B are the same
  int m01 = 0; //represents the total number when A and B are different

  //feed every letter of query and currentword into separate vectors
  vector<char> qstring;
  for(auto i : query){
    qstring.push_back(i);
  }
  vector<char> wstring;
  for(auto j : currentword){
    wstring.push_back(j);
  }

  //loop through every character in currentword and put them in one of these 2 categories (does it match a char in query or not)
  int len = (qstring.size() < wstring.size() ? qstring.size() : wstring.size());
  for(int i = 0; i < len; i++){
    if(wstring[i] == qstring[i]){ //check same index for matching letter
      // wstring[i].match = true;
      m11++;
    }
    else if(wstring[i] == qstring[i-1]){ //check letter behind (just in case it's misspelled/letter omitted)
      // wstring[i].match = true;
      m11++;
    }
    else if(wstring[i] == qstring[i+1]){ //check next letter (just in case it's misspelled/letter omitted)
      // wstring[i].match = true;
      m11++;
    }
    else{
      // wstring[i].match = false;
      m01++;
    }
  }

  //Jaccard similarity coefficient
  double J = (double)m11/qstring.size();

  return (1-J)*10;
}

vector<string> Searcher::parsePhrase(string query){
  vector<string> strVec;
  //int j = 0;

  stringstream ss(query);
  string word;
    while(ss >> word){
      //query[i] >> strArray[i];
      //strVec.push_back(query.substr(j, i-1)); //check if substr is copying properly
      strVec.push_back(word);
      //j = i+1;
    }

  return strVec;
}

int Searcher::calcWordDiff(vector<string> queryWords, string restofline){
  vector<string> rest = parsePhrase(restofline);
  int MAX_DIFF = wordDiff;
  // bool ismatch = false;
  int diffct = 0; //make sure you change this
  int lastmatch = 0;

  for(uint i = 0; i < queryWords.size(); i++){
    for(uint j = lastmatch; j <= i+MAX_DIFF-diffct; j++){
      if(queryWords[i] == rest[j]){
        lastmatch = j+1;
        break;
      }
      else{
        diffct++;
      }
    }
    if(diffct > MAX_DIFF){
      return diffct;
    }
  }

  return diffct;
}

void Searcher::printMatch(Match match){
  cout << endl << match.file << endl;
  cout << match.text << endl;
}
