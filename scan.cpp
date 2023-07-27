#include <vector>
#include <iostream>

#include "atel.hpp"
#include "defs.hpp"
#include "globals.hpp"

using namespace std;

vector<Token> tokens;

bool Scan::skip(char c) {
    switch (c) {
        case ' ':
        case '\r':
        case '\t':
            return true;
        case '\n':
            currentLine++;
            return true;
    }
        return false;
}

void Scan::addToken(TokenType tType, int iLiteral = 0, char* s = '\0') {
    currentToken.tokenType = tType;
    currentToken.intLiteral = iLiteral;
    currentToken.string = s;

    tokens.push_back(currentToken);
}

    void Scan::nextChar(void) {
        char c;
        if (prevChar) {
            currentChar = prevChar;
            prevChar = 0;
            return; 
        }
        c = fgetc(srcFile);
        if (c != EOF && !skip(c)) {
            currentChar = c;
            return ;    
        }

    };

vector<Token> Scan::scanToken(void) {
    while(currentChar != EOF) {
        switch (currentChar) {
            case '(': addToken(LEFT_PAREN); break;
            case ')': addToken(RIGHT_PAREN); break;
            case '{': addToken(LEFT_CURL); break;
            case '}': addToken(RIGHT_CURL); break;
            case '[': addToken(LEFT_BRACE); break;
            case ']': addToken(RIGHT_BRACE); break;
            case ',': addToken(COMMA); break;
            case '.': addToken(DOT); break;
            case '-': addToken(MINUS); break;
            case '+': addToken(PLUS); break;
            case '*': addToken(STAR); break;
            case '/': addToken(SLASH); break;
            case ';': addToken(SEMICOLON); break;
            case '!': {
                    addToken(match('=') ? BANG_EQ : BANG);
                }
                break;
            case '=': {
                    addToken(match('=') ? EQ_EQ : EQ);
                break;
                }
            case '<': {
                    addToken(match('=') ? LESS_EQ : LESS);
                break;
                }
            case '>': {
                    addToken(match('=') ? GREAT_EQ : GREAT);
                break;
                }
            case '"': handleString(); break;

            default: {
                printf("Syntax Error at token %d, line %d", currentChar, currentLine);
                exit(1);
            }
        }

    }
};

bool Scan::match(char c) {
    nextChar();
    if (currentChar != c) {
        return false;
    } else {
        return true;
    }
}

void Scan::handleString(void) {
    while (currentChar != '"') {
        nextChar();
    }
}