#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>
#include <map>

#include "atel.hpp"
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

void init();
void run();

int main(int arc, char *argv[]) {
    if (!argv[1]) {
        printf("Include source file as arg1\n");
        exit(1);
    }

    srcFile = fopen(argv[1], "r");
    if (srcFile == NULL) {
        printf("Could not open file");
        exit(1);
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
}