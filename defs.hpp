#include <vector>
#include <map>

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



std::map<std::string, int>  reservedWords = 
{
    {"true", TRUE},
    {"false", FALSE},
    {"while", WHILE},
    {"if", IF},
    //TO BE REPLACED WITH STRICT TYPES!
    {"var", VAR},
    {"return", RETURN},
    {"const", CONST},
    {"class", CLASS}

};

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
    bool skip(char c);
    bool match(char c);
    void handleString(void);
    void handleIntlit(void);
    void handleIdentifier(void);

    public:
    vector<Token> scanToken(void);
    void nextChar(void);

};