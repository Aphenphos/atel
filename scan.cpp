#include <vector>
#include <iostream>
#include <cstring>
#include <map>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

using namespace std;
char Scan::currentChar;
char Scan::prevChar;
char Scan::currentLiteral[512];
int Scan::currentLine;
vector<Token> Scan::tokens;
const map<string, TokenType> Scan::reservedWords = 
{
    {"true", TRUE},
    {"false", FALSE},
    {"while", WHILE},
    {"if", IF},
    {"else", ELSE},
    {"int", INT},
    {"return", RETURN},
    {"const", CONST},
    {"class", CLASS},
    {"for", FOR},
    {"void", VOID},
    {"char", CHAR},
    {"long", LONG}
};

bool Scan::skipChar() {
    switch (currentChar) {
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

void Scan::addToken(TokenType tType, int iLiteral, char* s) {
    currentToken.tokenType = tType;
    currentToken.literal.intLiteral = iLiteral;
    currentToken.literal.string = s;

    tokens.push_back(currentToken);
}

void Scan::nextChar(void) {
    char c;
    if (prevChar) {
        currentChar = prevChar;
        prevChar = '\0';
        return; 
    }
    c = fgetc(srcFile);
    currentChar = c;   
    return;
};

vector<Token> Scan::scanToken(void) {
    while(1) {
        if (skipChar() == true) {
            nextChar();
            continue;
        }
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
            case '|': {
                if (match('|')) {
                    addToken(OR); break;
                }  else {
                    handleScanningError();
                    break;
                }
            }
            case '&': {
                 addToken(match('&') ? AND : REF); 
                 break;
            }
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
            case EOF: {
                addToken(END);
                fclose(srcFile);
                return tokens;
            }
            default: {
                if (isdigit(currentChar)) {
                    handleIntlit();
                    break;
                } else if (isalpha(currentChar)) {
                    handleIdentifier();
                    break;
                }
                handleScanningError();
            }
        }
        nextChar();
    }
    return tokens;
};

bool Scan::match(char c) {
    nextChar();
    if (currentChar != c) {
        prevChar = currentChar;
        return false;
    } else {
        return true;
    }
}

void Scan::handleString(void) {
    int i = 1;
    currentLiteral[0] = currentChar;
    nextChar();
    while (currentChar != '"') {
        currentLiteral[i] = currentChar;
        i++;
        nextChar();
    }
    currentLiteral[i] = currentChar;
    currentLiteral[i + 1] = '\0';
    char* thisString = (char*)malloc(sizeof(i+1));
    strcpy(thisString, currentLiteral);
    addToken(STRING,0,thisString);
    return;
}

void Scan::handleIntlit(void) {
    int i = 1;
    currentLiteral[0] = currentChar;
    nextChar();
    while (isdigit(currentChar) || currentChar == '.') {
        currentLiteral[i] = currentChar;
        i++;
        nextChar();
    }
    prevChar = currentChar;
    currentLiteral[i] = '\0';
    int finalValue;
    sscanf(currentLiteral, "%d", &finalValue);
    addToken(INTLIT, finalValue);
    return;
}

void Scan::handleIdentifier(void) {
    int i = 1;
    currentLiteral[0] = currentChar;
    nextChar();

    while(isalpha(currentChar) || currentChar == '_') {
        currentLiteral[i] = currentChar;
        i++;
        nextChar();
    }
    prevChar = currentChar;
    currentLiteral[i] = '\0';

    string stringCurrentLiteral(currentLiteral);

    if (reservedWords.count(currentLiteral) == 1) {
        addToken(reservedWords.at(stringCurrentLiteral));
        return;
    } else {
        char* ident = (char*)malloc(sizeof(i+1));
        strcpy(ident, currentLiteral);
        addToken(IDENT,0,ident);
        return;

    }

}

void Scan::printTokens(void) {
    cout << "Size of TokenList:" << tokens.size() << endl;
    cout << "Size of TokenList:" << tokens.size() << endl;
    for (int i = 0; i < tokens.size(); i++) {
        tokens[i].print();
    }
    if (errors.size() != 0) {
        printErrors();
        exit(1);
    }

    return;
}