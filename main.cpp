//  CS 141 Spring 2019
//  Author: *** Farooq Syed ***
//
//  Open up a data file and find a starting point for some set of consecutive words
//  and print the starting word.
//
//  Next find all occurrences in the datafile of those consecutive words, and collect the set
//  of words that immediately follow each of those occurrences.  Choose one of the words
//  in this newly created set and add it to the set of consecutive words, shifting them
//  over by one so the first word gets overwritten by the second, and so on.
//
//  Repeat this process until >=30 words are displayed and an end-of-sentence word is found,
//  or until a displayed word has a newline or a return character at the end of it.
//


#include <iostream>
#include <cctype>  // For the letter checking functions
#include <fstream> // For file input
#include <vector>
#include <sstream>
#include <cstring> // For strlen()
#include <cstdlib> // For rand()
#include <utility>
#include <string.h>
using namespace std;

//---------------------------------------------------------------------
void getInputWords( vector<string> &allWords, vector<string> &startWords ) // List of all the words in the selected input file
{
  // Prompt for and get user choice for which data file to use
  cout << "Menu Options: \n"
    << "  1. Use Trump tweets \n"
    << "  2. Use Dalai Lama tweets \n"
    << "  3. Use Mixed Trump / Dalai Lama tweets \n"
    << "  4. Use small test file \n"
    << "  5. Use tiny test file \n"
    << "Your choice: ";
  int dataSelectionOption;
  cin >> dataSelectionOption;

  ifstream inputFileStream; // declare the input file stream
  // open input file and verify
  switch ( dataSelectionOption )
  {
    case 1:
      inputFileStream.open( "TrumpTweets.txt" );
      break;
    case 2:
      inputFileStream.open( "DalaiLamaTweets.txt" );
      break;
    case 3:
      inputFileStream.open( "TrumpLama.txt" );
      break;
    case 4:
      inputFileStream.open( "Test.txt" );
      break;
    case 5:
      inputFileStream.open( "tiny.txt" );
      break;
    default:
      cout << "Invalid option chosen, exiting program. ";
      exit( -1 );
      break;
  }
  // file doesn't exit let's throw msg and error out
  if ( !inputFileStream.is_open( ) )
  {
    cout << "Could not find input file.  Exiting..." << endl;
    exit( -1 );
  }

  char c = ' ';
  // assume all words are 81 char max length
  std::stringstream inputWord;
  //char inputWord[ 81 ];

  // Read a character at a time from the input file, separating out individual words.
  //inputWord[ 0 ] = '\0';
  int index = 0;
  bool startWord = true;
  while ( inputFileStream.get( c ) )
  {
    if ( !isprint( c ) && c != '\r' && c != '\n' )
    {
      continue; // Skip non-printable characters and get the next one
    }
    if ( c != ' ' )
    {
      // If it is not a return or newline, add it to the string.
      // If it is a return or newline character, only add it if there
      //    are already other characters in the string.
      if ( ( c != '\r' && c != '\n' ) ||
          ( ( c == '\r' || c == '\n' ) && index > 0 ) )
      {
        inputWord << c;
        //inputWord[ index++ ] = c;
      }
    }
    // End the word when encountering a space or a return character.
    if ( c == ' ' || c == '\r' || c == '\n' )
    {
      // Null terminate the input word. Store it if its length is > 0 and it is printable.
      //inputWord[ index ] = '\0';
      //auto inputString1 = inputWord.str( );
      std::string inputWord1 = inputWord.str( );
      const char* inputString = inputWord1.c_str( );
      if ( strlen( inputString ) > 0 && isprint( inputString[ 0 ] ) )
      {
        allWords.push_back( inputString);
        if ( startWord )
        {
          startWord = false;
          startWords.push_back(inputString);
        }
        inputWord.str( "" );
      }
      // Check for special case where there is a space at the end of the line.  We don't want
      // to lose the end of line character ('\n' or '\r'), so we concatenate it to the
      // last word that was previously stored on the list.  First check to see if this is the case:
      if ( ( c == '\r' || c == '\n' ) /*&& strlen( inputString ) == 0*/ )
      {
        long indexOfLastWord = allWords.size( ) - 1;
        allWords.at( indexOfLastWord ).append( "\r" );
        startWord = true;
      }

      index = 0; // Restart the index for the next word
    }
  } //end while( inputFileStream...)
}

//---------------------------------------------------------------------
void debug( vector<string> startWords, // List of first words in sentences
    vector<string> allWords,   // All words from input file
    vector<string> wordList,   // List of all words following search phrase
    string nextWord,
    int wordWindowWidth,
    int rng)           // Nexts word found after search phrase
{
  int debugMenuOption = 0;

  do
  {
    // Prompt for and get user choice for which debug option to use
    cout << " \n"
      << "    >>> Debug menu options: <<<\n"
      << "        1. Display a start word \n"
      << "        2. Display one of all words \n"
      << "        3. Display wordWindow and next words \n"
      << "        4. Exit debugging \n"
      << "    Your choice -> ";
    cin >> debugMenuOption;

    int wordIndexValue = 0; // Used to read user input for debug options below

    switch ( debugMenuOption )
    {
      case 1:
        cout << "    Enter an index value from 0 to " << startWords.size( ) - 1 << " to display a start word: ";
        cin >> wordIndexValue;
        cout << "    " << startWords.at( wordIndexValue ).c_str( ) << endl;
        break;
      case 2:
        cout << "    Enter an index value from 0 to " << allWords.size( ) - 1 << " to display one of all words: ";
        cin >> wordIndexValue;
        cout << "    " << allWords.at( wordIndexValue ).c_str( ) << endl;
        break;
      case 3:
        cout << "    WordWindow and next words are: ";
        // random number between 0 and max word count - (word window width + 1)
        string* words = new string[ wordWindowWidth ];
        for ( int i = rng; i < wordWindowWidth + rng; i++ )
        {
          words[ i - rng ] = allWords.at( i );
          cout << allWords.at(i) << " ";

        }
        for ( int i = 0; i < allWords.size(); i++ )
        {
          for ( int j = 0; j < wordWindowWidth; j++ )
          {
            if ( allWords.at( i ) != words[ j ] )
            {
              break;
            }
            i++;
            if ( j == wordWindowWidth - 1 )
            {
              wordList.push_back( allWords.at( i ) );
            }
          }
        }
        // Find a random starting word in the startWords list
        // ...
        // after all this we have generated a list of wordList - pick at random and set nextWord = random(nextWords);
        nextWord = wordList.at(rand() % wordList.size());
        cout << nextWord.c_str( );
        break;
    } //end switch(...)
  } while ( debugMenuOption != 4 );
  exit(0);

} //end debug()

/*void continueFn(vector<string> allWords, vector<string> startWords) {
  int i = 0;
  startWords.push_back(allWords.at(0));
  while(i != allWords.size() -1) {
    
    string word = allWords.at(i);
    char lastChar = word[word.size() - 1]
    if (i + 1 >= allWords.size())
    {
      break;
    }
    if (lastChar == '/r' || lastChar == '/n')
    {
      startWords.push_back(allWords.at(i + 1));
    }
    i++;
  }
  for (int i = 0; i < startWords.size(); i++)
  {
    cout << startWords.at(i)
         << " ";
  }
} */

// *** Yuwei: add header for this function to explain its purpose.
void continueFn(int wordWindowWidth, vector<string> allWords, vector<string> startWords) {
  int i = 0;
  startWords.push_back(allWords.at(0));
  while(allWords.at(i) != startWords.at(wordWindowWidth - 1)) {
    i++;
  }
  vector<string> lineWords;
  string word = allWords.at(i);
  const char* location = NULL;
  do {
    location = strchr(word.c_str(),'\n');
    if(location == NULL) {
      location = strchr(word.c_str(),'\r');
    }
    lineWords.push_back(word);
    word = allWords.at(++i);
  }while(location == NULL);
  for(i = 0; i < lineWords.size(); i++) 
  {
    cout << lineWords.at(i); 
    cout << " ";
  }
  cout << endl;
}

//---------------------------------------------------------------------
int main( )
{
  vector<string> allWords;   // List of all input words
  vector<string> startWords; // List of words that start sentences, to use
  //   in starting generated tweets.
  vector<string> wordList; // Stores the list of consecutive words that are searched for

  cout << "Welcome to the tweet generator. " << endl
    << endl;

  getInputWords( allWords, startWords );
  // You must write the function shown below, and call it from here:
  //getStartWords( startWords, allWords );

  string nextWord;

  // Allow looping to give more than one tweet
  int wordWindowWidth = 0;
  int lineToPrint = 0;
  char userInput = ' ';

// *** Yuwei: code within the same block should line up, and indented properly.  
    int rng_first_word_word_window =  rand() % allWords.size(); 
  // Main loop
  while ( userInput != 'X' )
  {
    cout << endl;
    cout << "Enter x to exit, or d/c to debug/continue followed by word window size: ";
    cin >> userInput;
    userInput = toupper( userInput ); // fold user input into upper case

    if ( userInput == 'X' )
    {
      break; // Exit program
    }
    // Read in the wordWindowWidth to be used in both the "continue" and the "debug" modes
    cin >> wordWindowWidth;
    lineToPrint += wordWindowWidth;
    // Also check for input of 'D' to debug
    // ...
    if ( userInput == 'D' )
    {
      debug(startWords, allWords, wordList, nextWord, wordWindowWidth, rng_first_word_word_window);
    } else if(userInput == 'C') {
      continueFn(lineToPrint,allWords, startWords);
    }
  } // end while( userInput != 'x')

  cout << endl
    << endl;

  return 0;
} //end main()
