
#include <iostream>
#include <vector>
#include <string>

#include "atel.hpp"
#include "defs.hpp"
#include "globals.hpp"
#include "handleErr.hpp"


using namespace std;


vector<string> errors;

void handleScanningError(void) { 
    char currentErrChar = currentChar;
    int currentErrLine = currentLine;

    errors.push_back("Line: " + to_string(currentErrLine) + " Char: " + "'" + string(1,currentErrChar) + "'");
}

void handleCustomError(char message[]) {
    printf(message);
}

void handleFatalError(char message[]) {
    printf(message);
    exit(1);
}


void printErrors(void) {
    if (errors.size()) {
        cout << "Syntax Errors" << endl;
        for (int i = 0; i < errors.size(); i++) {
            cout << errors[i] << endl;
        }
        cout << "______________________" << endl;
    }
    return;
}