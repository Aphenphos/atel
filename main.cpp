#include <string>
#include <fstream>
#include <stdio.h>
#include <iostream>

#include "atel.hpp"
#include "defs.hpp"
#include "globals.hpp"

using namespace std;
char currentChar;
char prevChar;
int currentLine;
Token currentToken;
char currentText[512];
FILE *srcFile;

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
}

void init() {
    currentLine = 1;
}
void run() {
    Scan scanner;
    scanner.nextChar();
    vector<Token> tokenList = scanner.scanToken();
    for (int i = 0; i < tokenList.size(); i++) {
        cout << tokenList[i].tokenType << endl;
    }
}