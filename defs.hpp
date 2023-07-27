#include <vector>

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
    TRUE, FALSE,
    VAR, CONST,

    EOF

};

class Token {
    public:
    TokenType tokenType;
    int intLiteral;
    char* string;

    Token(TokenType tType, int iLit, char* s) {
        tokenType = tType;
        intLiteral = iLit;
        string = s;
    }
};

class Scan {
    private:
    vector<Token> tokens;
    
    public:

    void nextChar(void);
};