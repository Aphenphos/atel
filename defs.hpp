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
        cout << tokenType << endl;
        if (string != nullptr) {
            cout << string << endl;
        }
        if (intLiteral) {
            cout << intLiteral << endl;
        }
        
    }
};

class Scan {
    private:
    vector<Token> tokens;
    void addToken(TokenType tType, int iLiteral = 0, char* s = nullptr); 
    bool skipChar(char c);
    bool match(char c);
    void handleString(void);
    void handleIntlit(void);
    void handleIdentifier(void);

    public:
    vector<Token> scanToken(void);
    void nextChar(void);

};

class Parse {
    vector<Token> tokens;
    int current = 0;

    Parse(vector<Token> tokens);

};

class Expression {
    public:
    Expression equality(); 
    Expression comparison();
    Token prevToken();
    bool match(TokenType types...);
    bool check(TokenType t);
    bool next(void);
    Expression additive();
    Expression multiplicative();
    Expression unary();
};

class Binary: public Expression {
    
    Expression left;
    Token op;
    Expression right;
    public:
    Binary(Expression left, Token op, Expression right);
};

class Grouping: public Expression {
    Expression expression;
    Grouping(Expression expression);
};

class Literal: public Expression {
    int intLiteral;
    char* string;

    Literal(char* string = nullptr, int intLiteral = 0);
};

class Unary: public Expression {
    Token op;
    Expression right;

    Unary(Token op, Expression right);
};
