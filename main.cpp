#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <map>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

using namespace std;
char currentChar;
char prevChar;
int currentLine;
Token currentToken;
Token prevToken;
char currentLiteral[512];
FILE *srcFile;
map<string, TokenType> reservedWords = 
{
    {"true", TRUE},
    {"false", FALSE},
    {"while", WHILE},
    {"if", IF},
    {"int", INT},
    {"return", RETURN},
    {"const", CONST},
    {"class", CLASS},
    {"print", PRINT}

};


void init();
void run();



int main(int arc, char *argv[]) {
    if (!argv[1]) {
        char err[] = "please provide sourcefile";
        handleFatalError(err);
    }

    srcFile = fopen(argv[1], "r");
    if (srcFile == NULL) {
        char err[] = "Could not open sourcefile";
        handleFatalError(err);
    }

    init();
    run();
    exit(0);
}

void init() {
    currentLine = 1;
}

void run() {
    Scan scanner;
    scanner.nextChar();
    vector<Token> tokenList = scanner.scanToken();
    scanner.printTokens();

    vector<Token>* tokensPointer = &scanner.tokens;
    
    Parse::initParser(tokensPointer);

    Asm::init((char *)"test.nasm");
    Asm::preamble();
    Statement::statements();
    Asm::postamble();
}