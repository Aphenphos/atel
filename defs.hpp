#pragma once

#include <vector>
#include <map>
#include <stdio.h>
#include <iostream>

using namespace std;

enum TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_CURL, RIGHT_CURL,
    LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS, PLUS, 
    SEMICOLON, SLASH, STAR,

    BANG, BANG_EQ,
    EQ, EQ_EQ,
    GREAT, GREAT_EQ,
    LESS, LESS_EQ,

    ASSIGN, INT, LVIDENT, IDENT, STRING, INTLIT,

    CLASS,

    AND, ELSE, TRUE, FALSE,
    FOR, IF, OR, WHILE, RETURN,
    VAR, CONST,


    PRINT,

    END
};



extern map<string, TokenType> reservedWords;
void matchTokenAndString(TokenType type, char* string);

struct Token {
    TokenType tokenType;
    union {
        int intLiteral;
        char* string;
    } literal;

    void print(void) {
        printf("TokenType: %d \n", tokenType);
        if (literal.string != nullptr) {
            printf("stringLit: ");
            cout << literal.string << endl;
        }
        if (literal.intLiteral) {
            printf("IntLit: %i \n", literal.intLiteral);
        }
        
    }
};

class Scan {
    private:
    void addToken(TokenType tType, int iLiteral = 0, char* s = nullptr); 
    bool skipChar(void);
    bool match(char c);
    void handleString(void);
    void handleIntlit(void);
    void handleIdentifier(void);

    public:
    void printTokens(void);
    vector<Token> tokens;
    vector<Token> scanToken(void);
    void nextChar(void);

};

struct opPrecObj {
    TokenType type;
    int prec;
};

class Expression {
    private:
    static map<TokenType, int> opPrecValues;

    public:
    Expression* left;
    Expression* right;
    TokenType op;
    union {
        int intValue;
        int id;
    } value;

    Expression(Expression* left, Expression* right, TokenType op, int intValue);
    static Expression* init(void);
    static Expression* castPrimary(void);
    static Expression* binaryExpression(int prevTokenPrec);
    static Expression* castLeaf(TokenType op, int intValue);
    static Expression* castUnary(TokenType op, Expression* left, int intValue);
    static int getOpPrec(TokenType type);
};

class Statement {
    public:
    static void statements(void);
    static void printStatement(void);
    static void assignmentStatement(void);
    static void varDeclaration(void);
};

class Parse {
    private:
    static vector<Token> tokens;

    public:
    static int current;
    static void initParser(vector<Token>* tokens);
    static void nextToken(void);
    static void parse(void);
    static int interpretAST(Expression* tree, int r);
};

class Symbols {
    public:
    struct Symbol {
        char* name;
    };
    static int globalSymbolsCount;
    static Symbol symbolTable[1024];
    static int findGlobalSymbol(char* s);
    static int newGlobalSymbol(void);
    static int addGsymbol(char* s);


    
};


class Asm {
    private:
    static FILE *outfile;

    public:
    static void init(char* filename);
    static int freeRegisters[4];
    const static char* registerList[4];

    static void freeAllRegisters(void);
    static int allocateRegister(void);
    static void freeRegister(int r);
    static void preamble(void);
    static void postamble(void);

    static int loadInt(int value);
    static int add(int r1, int r2);
    static int subtract(int r1, int r2);
    static int multiply(int r1, int r2);
    static int divide(int r1, int r2);

    static void globalSymbol(char* s);
    static int loadGlobalSymbol(char* s);
    static int storeGlobalSymbol(int r, char* s);

    static void printInt(int r);
};