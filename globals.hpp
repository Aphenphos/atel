#pragma once


#define cp (char*)

extern char currentChar;
extern char prevChar;
extern int currentLine;
extern char currentLiteral[512];
extern Token currentToken;
extern Token prevToken;
extern FILE *srcFile;