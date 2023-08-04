#include <vector>
#include <iostream>
#include <cstring>
#include <map>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

vector<Token> Parse::tokens;
int Parse::current;

map<TokenType, int> Expression::opPrecValues = 
{   
    {END , 0}, 
    {INTLIT, 0}, 
    {PLUS, 10}, 
    {MINUS, 10}, 
    {STAR, 20}, 
    {SLASH, 20}
};

Expression::Expression(Expression* pleft, Expression* pright, TokenType pop, int pintValue) {
    left = pleft;
    right = pright;
    op = pop;
    value.intValue = pintValue;
}


void Parse::initParser(vector<Token>* ts) {
    cout << "_________Parser____________"<<endl;
    Parse::tokens = *ts;
    Parse::current = 0;
    currentToken = tokens[current];
}

void Parse::nextToken(void) {
    current++;
    currentToken = tokens[current];
}

int Parse::interpretAST(Expression* n, int r) {
    int leftRegister, rightRegister;
    if (n->left) {
        leftRegister = Parse::interpretAST(n->left, -1);
    }
    if (n->right) {
        rightRegister = Parse::interpretAST(n->right, leftRegister);
    }

    switch(n->op) {
        case PLUS:
            return Asm::add(leftRegister, rightRegister);
        case MINUS:
            return Asm::subtract(leftRegister, rightRegister);
        case STAR:
            return Asm::multiply(leftRegister, rightRegister);
        case SLASH:
            return Asm::divide(leftRegister, rightRegister);
        case INTLIT:
            return Asm::loadInt(n->value.intValue);
        case IDENT:
            return Asm::loadGlobalSymbol(Symbols::symbolTable[n->value.id].name);
        case LVIDENT:
            return Asm::storeGlobalSymbol(r, Symbols::symbolTable[n->value.id].name);
        case ASSIGN:
            return(rightRegister);
        default:
            fprintf(stderr, "Parsing error %d\0\n", n->op);
            exit(1);
    }

}

Expression* Expression::castLeaf(TokenType op, int intValue) {
    return(new Expression(nullptr, nullptr, op, intValue));
}

Expression* Expression::castUnary(TokenType op, Expression* left, int intValue) {
    return (new Expression(left, nullptr, op, intValue));
}

Expression* Expression::castPrimary(void) {
    Expression* node;
    int id;
    switch(currentToken.tokenType) {
        case INTLIT:
            node = castLeaf(INTLIT, currentToken.literal.intLiteral);
            break;
        case IDENT:
            id = Symbols::findGlobalSymbol(currentToken.literal.string);
            if (id == -1) {
                handleUnknownVar();
            }
            node = castLeaf(IDENT, id);
            break;
        default:
            fprintf(stderr, "Parsing error %d", node->op);
            exit(1);
    }
    Parse::nextToken();
    return node;
}

Expression* Expression::binaryExpression(int prevTokenPrec) {
    Expression* left, *right;
    TokenType type;
    left = castPrimary();
    type = currentToken.tokenType;

    if (type == SEMICOLON) {
        return(left);
    }

    while(getOpPrec(type) > prevTokenPrec) {
        Parse::nextToken();
        right = binaryExpression(opPrecValues.at(type));
        left = new Expression(left, right, type, 0);

        type = currentToken.tokenType;
        if (type == SEMICOLON) {
            return (left);
        }

    }
    return left;
}


Expression* Expression::init(void) {
    return binaryExpression(0);
}

int Expression::getOpPrec(TokenType type) {
    int prec = opPrecValues.at(type);
    if (prec == 0) {
        fprintf(stderr, "syntax error on line:%d token: %d", currentLine, type);
        exit(1);
    }

    return prec;
}
