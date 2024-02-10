
#include <unistd.h>
#include <stdio.h>
#include <stdio.h>
#include <ctype.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std; 

#define MAX_BUFFER_SIZE 1024*1024

/* String functions
==========================
*/


// split string p_line into a vector of strings (words)
// the delimiters are 1 or more whitespaces
std::vector<std::string> split( const std::string & p_line)
{
  auto line = p_line + " ";
  std::vector<std::string> res;
  bool in_str = false;
  std::string curr_word = "";
  for( auto c : line) {
    if( isspace(c)) {
      if( in_str)
        res.push_back(curr_word);
      in_str = false;
      curr_word = "";
    }
    else {
      curr_word.push_back(c);
      in_str = true;
    }
  }
  return res;
}


/*Buffer and read functions
===============================
*/

//The following function contains code written by Pavol Federl(2020), at https://gitlab.com/cpsc457p21/longest-int-my-getchar/-/blob/main/fast-int.cpp

// we'll use global variables to track of characters that we
// read() into memory
char biggerBuffer[MAX_BUFFER_SIZE]; // 1MB storage to hold results of read()
int buff_size = 0;      // stores how many characters are stored in biggerBuffer
int buff_pos = 0;       // position in biggerBuffer[] where we extract next char

int endOfFile; //1 if EOF reached

char currentC;
string line;


// returns the next char from stdin, or -1 on EOF
// returns characters stored in biggerBuffer[] until it is empty
// when biggerBuffer[] gets empty, it is replenished by calling read()
// returns 0 when buffer is full?
// returns 1 normally?
// must keep track of whether biggerBuffer ends in the middle of a word, otherwise is_palindrome won't work on cut-off words
void fastReadOneChar()
{
  //cout << "buff_pos: " << buff_pos << endl; 
  // check if biggerBuffer[] is empty
  if( buff_pos >= buff_size) {
    // biggerBuffer is empty, let's replenish it 
    buff_size = read( STDIN_FILENO, biggerBuffer, sizeof(biggerBuffer));

    // detect EOF
    if(buff_size <= 0) {
      endOfFile = 1; 
      return;
    }
    // reset position from where we'll return characters
    buff_pos = 0;
  }
  // return the next character from the buffer and update position
  //return buffer[buff_pos++];

  //increment position in biggerBuffer, and print out character that was read in 
  currentC = biggerBuffer[buff_pos++];

}


// ========= Palindrome operations ==========

// returns true if a word is palindrome
// palindrome is a string that reads the same forward and backward

bool is_palindrome( const std::string & s)
{
  for( size_t i = 0 ; i < s.size() / 2 ; i ++)
    if( tolower(s[i]) != tolower(s[s.size()-i-1]))
      return false;
  return true;
}

// takes the line currently being read and returns the longest palindrome on standard input
// in case of ties for length, returns the first palindrome
// all input is broken into words, and each word is checked
// word is a sequence of characters separated by white space
// white space is whatever isspace() says it is
//    i.e. ' ', '\n', '\r', '\t', '\n', '\f'
std::string longestPalindrome()
{
  string maxPali = "";
  //cout << "determining longest palindrome" << endl; 

  if( line.size() == 0) return maxPali;
  auto words = split(line);
  for( auto word : words) {
    if( word.size() <= maxPali.size()) continue;
    if( is_palindrome(word))
      maxPali = word;
    }
  return maxPali;
}

int
main()
{
  endOfFile = 0; 
  //cout << "current max buffer size is: " << MAX_BUFFER_SIZE << endl; 

  string longP;  
  string result; 
 
  while (true){ //read in file 
    line = ""; 

      while(true){ //read in line 
      fastReadOneChar();

      if (endOfFile == 1){ //EOF reached
        //endOfFile = 1; 
        break; 
      }
  
      if (currentC  == '\n'){
        break; 
      }
      
      line.push_back(currentC); 
    }
      
    result = longestPalindrome(); 
    if (longP.size() < result.size()) longP = result; 

    if (endOfFile == 1) break; 

  }

  cout << "Longest palindrome: " << longP << endl; 

  return 0;
}

