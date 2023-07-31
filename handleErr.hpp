#pragma once

#include <vector>
#include <string>

using namespace std;

extern vector<string> errors;

void handleSyntaxError(void);
void handleCustomError(char message[]);
void handleFatalError(char message[]);
void printErrors(void);