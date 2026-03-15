#ifndef TOKENIZER_H
#define TOKENIZER_H

#include <string>
#include <fstream>
using namespace std;

// Tokenizer class
// Provides rutines to skip comments and white space, get the next token and advance to the next token.
class Tokenizer {
public:
    string currentToken;
    bool hasMore = true;
    // Constructor: Open the input .jack file and get it ready to tokenize it
    Tokenizer(const string& filename);

    // hasMoreTokens: Are there more tokens in the input? (Y/N)
    bool hasMoreTokens();

    // advance: Get the next token from the input and make it the current token
    void advance();

    // tokenType: Return the type of the current token
    string tokenType();

    // keyWord: If the token is keyword type return the keyword
    string keyWord();

    // symbol: Return the char in case the token is symbol type
    char symbol();

    // identifier: Return a string for identifier type tokens
    string identifier();

    // intVal: Return the interger value for INT_CONST tokens
    int intVal();

    // stringVal: Return the string value of the current token in case it is STRING_CONST type
    string stringVal();

private:
    ifstream inputFile;
};

#endif // TOKENIZER_H
