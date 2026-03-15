#include "JackTokenizer.h"
#include <iostream>
#include <algorithm>

// Constructor: Open the input .jack file and get it ready to tokenize it
Tokenizer::Tokenizer(const string& filename) {
    inputFile.open(filename);
}

// hasMoreTokens: Are there more tokens in the input? (Y/N)
bool Tokenizer::hasMoreTokens() {
    return hasMore;
}

// advance: Get the next token from the input and make it the current token
void Tokenizer::advance() {
    string token;
    char character;
    string dummy;
    string symbols[] = {"{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/", "&", "|", "<", ">", "=", "~"};
    bool symbolFound;

    // Skip whitespace and comments
    while (inputFile.get(character)) {
        // Skip whitespace
        if (isspace(character)) continue;

        // Skip single-line comment
        if (character == '/' and inputFile.peek() == '/') {
            getline(inputFile, dummy);  // skip rest of line
            continue;
        }

        // Skip multi-line comment
        if (character == '/' and inputFile.peek() == '*') {
            while ((character != '*' or inputFile.peek() != '/') and !inputFile.eof()) {
                inputFile.get(character);
            }
            inputFile.get(character);
            continue;
        }

        // Found something meaningful
        break;
    }

    if (inputFile.eof()) {
        hasMore = false;
        return;
    }
    symbolFound = find(symbols, symbols + 19, string(1, character)) != symbols + 19;
    // If it's a symbol, it's a token by itself
    if (symbolFound) {
        currentToken = string(1, character);
        return;
    }

    if (character == '"') {
        // Start of a string constant
        token += character;
        while (true) {
                inputFile.get(character);
                if (character == '"') {
                        token += character;
                        break;
                }
                token += character;
        }
        currentToken = token;
        return;
    }

    // Until we get to a whitespace or a symbol every character is part of the same token
    while (!isspace(character) and !symbolFound) {
        token += character;
        inputFile.get(character);
        symbolFound = find(symbols, symbols + 19, string(1, character)) != symbols + 19;
    }

    // If we stopped on a symbol, put it back
    if (symbolFound) {
        inputFile.unget();
    }

    if (token.empty()) {
        hasMore = false;
    }
    else {
        currentToken = token;
    }
}

// tokenType: Return the type of the current token
string Tokenizer::tokenType() {
    string type;
    string keyWords[] = {"class", "constructor", "function", "method", "field", "static", "var", "int", "char", "boolean", "void",
                        "true", "false", "null", "this", "let", "do", "if", "else", "while", "return"};
    string symbols[] = {"{", "}", "(", ")", "[", "]", ".", ",", ";", "+", "-", "*", "/", "&", "|", "<", ">", "=", "~"};

    if (find(keyWords, keyWords + 21, currentToken) != keyWords + 21) {
        type = "KEYWORD";
    }

    else if (find(symbols, symbols + 19, currentToken) != symbols + 19) {
        type = "SYMBOL";
    }

    else if (currentToken[0] == '"' and currentToken[currentToken.size() - 1] == '"') {
        type = "STRING_CONST";
    }
    else{
        for (char c : currentToken) {
            if (isdigit(c)) {
                type = "INT_CONST";
            }
            else {
                type = "IDENTIFIER";
                break;
            }
        }
    }
    return type;
}

// keyWord: If the token is keyword type return the keyword
string Tokenizer::keyWord() {
    if (tokenType() == "KEYWORD") {
        return currentToken;
    }
    return "";
}

// symbol: Return the char in case the token is symbol type
char Tokenizer::symbol() {
    if (tokenType() == "SYMBOL") {
        return currentToken[0];
    }
    return '0';
}

// identifier: Return a string for identifier type tokens
string Tokenizer::identifier() {
    if (tokenType() == "IDENTIFIER") {
        return currentToken;
    }
    return "";
}

// intVal: Return the integer value for INT_CONST tokens
int Tokenizer::intVal() {
    if (tokenType() == "INT_CONST") {
        return stoi(currentToken);
    }
    return 0;
}

// stringVal: Return the string value of the current token in case it is STRING_CONST type
string Tokenizer::stringVal() {
    if (tokenType() == "STRING_CONST") {
        // We skip the openning and closing double quotes
        return currentToken.substr(1, currentToken.size() - 2);
    }
    return "";
}

