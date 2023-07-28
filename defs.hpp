#include <vector>


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


};

struct Token {
    TokenType tokenType;
    int intLiteral;
    char* string;
};

class Scan {
    private:
    vector<Token> tokens;
    void addToken(TokenType tType, int iLiteral = 0, char* s = nullptr); 
    bool skip(char c);
    bool match(char c);
    void handleString(void);
    
    public:
    vector<Token> scanToken(void);
    void nextChar(void);

};