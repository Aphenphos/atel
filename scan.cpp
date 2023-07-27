#include <vector>
#include <iostream>

#include "atel.hpp"
#include "defs.hpp"
#include "globals.hpp"

using namespace std;

class Scan {
    private:
    vector<Token> tokens;

    bool skip(char c) {
        switch (c) {
            case ' ':
            case '\r':
            case '\t':
                nextChar();
                return true;
            case '\n':
                currentLine++;
                return true;
        }
        return false;
    }

    void addToken(TokenType tType, int intLiteral, char* string) {
        Token newToken(tType, intLiteral, string);
        tokens.push_back(newToken);
    }

    
    public:
    void nextChar(void) {
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

    vector<Token> scanTokens(void) {
        while(currentChar != EOF) {
            switch (currentChar) {
                case '(': addToken(LEFT_PAREN, NULL, NULL); break;
                case ')': addToken(RIGHT_PAREN, NULL, NULL); break;
                case '{': addToken(LEFT_CURL, NULL, NULL); break;
                case '}': addToken(RIGHT_CURL, NULL, NULL); break;
                case '[': addToken(LEFT_BRACE, NULL, NULL); break;
                case ']': addToken(RIGHT_BRACE, NULL, NULL); break;
                case ',': addToken(COMMA, NULL, NULL); break;
                case '.': addToken(DOT, NULL, NULL); break;
                case '-': addToken(MINUS, NULL, NULL); break;
                case '+': addToken(PLUS, NULL, NULL); break;
                case '*': addToken(STAR, NULL, NULL); break;
                case ';': addToken(SEMICOLON, NULL, NULL); break;

                default: {
                     printf("Syntax Error at token %d, line %d", currentChar, currentLine);
                     exit(1);
                }
            }

        }
    };
};