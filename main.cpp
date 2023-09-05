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
FILE *Scan::srcFile;

Token currentToken;

void run();



int main(int arc, char *argv[]) {
    if (!argv[1]) {
        char err[] = "please provide sourcefile";
        handleFatalError(err);
    }

    Scan::srcFile = fopen(argv[1], "r");
    if (Scan::srcFile == NULL) {
        char err[] = "Could not open sourcefile";
        handleFatalError(err);
    }

    run();
    exit(0);
}


void run() {
    Scan::currentLine = 1;
    Scan::nextChar();
    vector<Token> tokenList = Scan::scanToken();
    // Scan::printTokens();

    vector<Token>* tokensPointer = &Scan::tokens;
    
    Parse::initParser(tokensPointer);

    Asm::init((char *)"test.asm");
    Asm::preamble();
    Symbols::addGsymbol(cp"printint", CHAR, FUNCTION, 0);
    Statement::globalDeclarations();
    Asm::postamble();
    return;
}