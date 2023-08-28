#pragma once

#include <vector>
#include <map>
#include <stdio.h>
#include <iostream>

using namespace std;

enum TokenType {
    EMPTY, LEFT_PAREN=1, RIGHT_PAREN, LEFT_CURL, RIGHT_CURL,
    LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS, PLUS, 
    SEMICOLON, SLASH, STAR, REF,

    BANG, BANG_EQ,
    EQ, EQ_EQ,
    GREAT, GREAT_EQ,
    LESS, LESS_EQ,

    INT, LONG, VOID, CHAR, STRING,
    INTPTR, LONGPTR, CHARPTR, VOIDPTR, STRINGPTR,
    ADDRESS, DEREF,
    
    LVIDENT, IDENT, INTLIT,

    CLASS, FUNCTION, FUNCCALL,

    AND, ELSE, TRUE, FALSE,
    FOR, IF, OR, WHILE, RETURN,
    VAR, CONST,


    PRINT,
    
    HOLDER, WIDEN, SCALE,


    END
};

const string TokenTypeArr[] = {
    "EMPTY","LEFT_PAREN","RIGHT_PAREN", "LEFT_CURL", "RIGHT_CURL",
    "LEFT_BRACE", "RIGHT_BRACE", "COMMA", "DOT", "MINUS", "PLUS", 
    "SEMICOLON", "SLASH", "STAR", "REF",

    "BANG", "BANG_EQ",
    "EQ", "EQ_EQ",
    "GREAT", "GREAT_EQ",
    "LESS","LESS_EQ",

    "INT", "LONG", "VOID", "CHAR", "STRING",
    "INTPTR", "LONGPTR", "CHARPTR", "VOIDPTR", "STRINGPTR",
    "ADDRESS", "DEREF"

    "LVIDENT", "IDENT",  "INTLIT",

    "CLASS", "FUNCTION", "FUNCCALL",

    "AND", "ELSE", "TRUE", "FALSE",
    "FOR", "IF", "OR","WHILE", "RETURN",
    "VAR","CONST",


    "PRINT",
    
    "HOLDER", "WIDEN", "SCALE",


    "END"
};




extern map<string, TokenType> reservedWords;
void checkCurToken(TokenType type);

struct Token {
    TokenType tokenType;
    struct {
        int intLiteral;
        char* string;
    } literal;

    void print(void) {
        cout << "Token Type: " + TokenTypeArr[tokenType] << endl;
        if (literal.string != nullptr) {
            printf("stringLit: ");
            cout << literal.string << endl;
        }
        if (literal.intLiteral) {
            printf("intLit: ");
            cout << literal.intLiteral << endl;
        }
        
    }
};

class Scan {
    private:
    const static map<string, TokenType> reservedWords;
    static void addToken(TokenType tType, int iLiteral = 0, char* s = nullptr); 
    static bool skipChar(void);
    static bool match(char c);
    static void handleString(void);
    static void handleIntlit(void);
    static void handleIdentifier(void);

    public:
    static FILE* srcFile;
    static char currentChar;
    static char prevChar;
    static char currentLiteral[512];
    static int currentLine;
    static void printTokens(void);
    static vector<Token> tokens;
    static vector<Token> scanToken(void);
    static void nextChar(void);

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
    Expression* middle;
    Expression* right;
    TokenType op, type;
    union {
        int intValue;
        int id;
        int size;
    } value;

    Expression(Expression* left, Expression* mid, Expression* right, TokenType op, TokenType type, int intValue);
    static Expression* init(void);
    static Expression* castPrimary(void);
    static Expression* binaryExpression(int prevTokenPrec);
    static Expression* castLeaf(TokenType op, TokenType type, int intValue);
    static Expression* castUnary(TokenType op, TokenType type, Expression* left, int intValue);
    static Expression* prefix(void);
    static int getOpPrec(TokenType type);
};

class Statement {
    public:
    static int currentFuncID;

    static void statements(void);
    static void globalDeclarations(void);
    static Expression* printStatement(void);
    static Expression* assignmentStatement(void);
    static void varDeclaration(TokenType type);
    static Expression* funcDeclaration(TokenType type);
    static Expression* compoundStatement(void);
    static Expression* singleStatement(void);
    static Expression* ifStatement(void);
    static Expression* whileStatement(void);
    static Expression* forStatement(void);
    static Expression* callFunction(void);
    static Expression* returnStatement(void);
};

class Parse {
    private:
    static vector<Token> tokens;
    static int labelCount;


    public:
    static int label();
    static int current;
    static void initParser(vector<Token>* tokens);
    static void nextToken(void);
    static Token prev(void);
    static Token peek(void);
    static void parse(void);
    static int genAST(Expression* tree, int r, TokenType parent);
    static int ifAST(Expression* n);
    static int whileAST(Expression* n);
};

class Symbols {
    public:
    struct Symbol {
        char* name;
        TokenType type;
        TokenType sType;
        int end;
    };
    static int globalSymbolsCount;
    static Symbol symbolTable[1024];
    static int findGlobalSymbol(char* s);
    static int newGlobalSymbol(void);
    static int addGsymbol(char* s, TokenType type, TokenType sType, int end);


    
};

class Types {
    const static map<TokenType, int> size;
    public:
    static bool compatible(TokenType &left, TokenType &right, bool o);
    static int getSize(TokenType type);
    static TokenType pointer(TokenType t);
    static TokenType pointerValue(TokenType t);
    static TokenType determine();
    static Expression* modifyType(Expression* tree, TokenType rType, TokenType op);
    static bool isInt(TokenType type);
    static bool isPtr(TokenType type);
};

class Asm {
    private:
    static FILE *outfile;
    static int compare(int r1, int r2, char* instruction);
    static int freeRegisters[4];
    const static char* registerList[4];
    const static char* bRegisterList[4];
    const static char* dRegisterList[4];
    const static char* compareList[6];
    const static char* jumpList[6];


    public:
    static void init(char* filename);

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

    static void globalSymbol(int id);
    static int loadGlobalSymbol(int id);
    static int storeGlobalSymbol(int r, int id);

    static int eq(int r1, int r2);
    static int bangEq(int r1, int r2);
    static int less(int r1, int r2);
    static int great(int r1, int r2);
    static int lessEq(int r1, int r2);
    static int greatEq(int r1, int r2);

    static void printInt(int r);

    static void funcPreamble(char* s);
    static void funcPostamble(int id);

    static void ret(int r, int id);
    static int call(int r, int id);

    static void jump(int r);
    static void label(int r);
    static int widen(int r);
    static int compareAndJump(TokenType instruction, int r1, int r2, int label);
    static int compareAndSet(TokenType instruction, int r1, int r2);

    static int address(int id);
    static int deref(int id, TokenType type);

    static int shiftLeft(int r, int pwr);
};