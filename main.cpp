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
char currentLiteral[512];
FILE *srcFile;
map<string, TokenType> reservedWords = 
{
    {"true", TRUE},
    {"false", FALSE},
    {"while", WHILE},
    {"if", IF},
    //TO BE REPLACED WITH STRICT TYPES! u8 u16 s8 s16 etc
    {"var", VAR},
    {"return", RETURN},
    {"const", CONST},
    {"class", CLASS}

};
vector<Token> Parse::tokens;
int Parse::current;



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
    vector<Token> tl = scanner.scanToken();
    cout << "Size of TokenList:" << tl.size() << endl;
    for (int i = 0; i < tl.size(); i++) {
        tl[i].print();
    }
    if (errors.size() != 0) {
        printErrors();
        exit(1);
    }
    cout << "_________Parser____________"<<endl;
    Parse::initParser(tl);
    Expression* node;
    node = Expression::init();
    printf("%d\n", Parse::interpretAST(node));
}