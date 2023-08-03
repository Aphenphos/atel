#pragma once

#include <vector>
#include <map>

using namespace std;

enum TokenType {
    LEFT_PAREN, RIGHT_PAREN, LEFT_CURL, RIGHT_CURL,
    LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS, PLUS, 
    SEMICOLON, SLASH, STAR,

    BANG, BANG_EQ,
    EQ, EQ_EQ,
    GREAT, GREAT_EQ,
    LESS, LESS_EQ,

    IDENT, STRING, INTLIT,

    CLASS,

    AND, ELSE, TRUE, FALSE,
    FOR, IF, OR, WHILE, RETURN,
    VAR, CONST,

    END
};



extern map<string, TokenType> reservedWords;

struct Token {
    TokenType tokenType;
    int intLiteral;
    char* string;

    void print(void) {
        printf("TokenType: %d \n", tokenType);
        if (string != nullptr) {
            printf("stringLit: \n", string);
        }
        if (intLiteral) {
            printf("IntLit: %i \n", intLiteral);
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
    vector<Token> tokens;
    vector<Token> scanToken(void);
    void nextChar(void);

};

struct opPrecObj {
    TokenType type;
    int prec;
};

class Expression {
    static map<TokenType, int> opPrecValues;

    public:
    Expression* left;
    Expression* right;
    TokenType op;
    int intValue;

    Expression(Expression* left, Expression* right, TokenType op, int intValue);
    static Expression* init(void);
    static Expression* castPrimary(void);
    static Expression* binaryExpression(int prevTokenPrec);
    static Expression* castLeaf(TokenType op, int intValue);
    static Expression* castUnary(TokenType op, Expression* left, int intValue);
    static int getOpPrec(TokenType type);
};

class Parse {
    public:
    static vector<Token> tokens;
    static int current;
    static void initParser(vector<Token> tokens);
    static void nextToken(void);
    static void parse(void);
    static int interpretAST(Expression* tree);
};
