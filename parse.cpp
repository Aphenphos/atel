#include <vector>
#include <iostream>
#include <cstring>
#include <map>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

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
    intValue = pintValue;
}


void Parse::initParser(vector<Token> ts) {
    Parse::tokens = ts;
    Parse::current = 0;
    currentToken = tokens[current];
}

void Parse::nextToken(void) {
    current++;
    currentToken = tokens[current];
}

int Parse::interpretAST(Expression* n) {
    int leftVal, rightVal;
    if (n->left) {
        leftVal = Parse::interpretAST(n->left);
    }
    if (n->right) {
        rightVal = Parse::interpretAST(n->right);
    }

    switch(n->op) {
        case PLUS:
            return leftVal + rightVal;
        case MINUS:
            return leftVal - rightVal;
        case STAR:
            return leftVal * rightVal;
        case SLASH:
            return leftVal / rightVal;
        case INTLIT:
            return n->intValue;
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
    switch(currentToken.tokenType) {
        case INTLIT:
            node = castLeaf(INTLIT, currentToken.intLiteral);
            Parse::nextToken();
            return node;
        default:
            fprintf(stderr, "Parsing error %d", node->op);
            exit(1);
    }
}

Expression* Expression::binaryExpression(int prevTokenPrec) {
    Expression* left, *right;
    TokenType type;
    left = castPrimary();
    type = currentToken.tokenType;

    if (type == END) {
        return(left);
    }

    while(getOpPrec(type) > prevTokenPrec) {
        Parse::nextToken();
        right = binaryExpression(opPrecValues.at(type));
        left = new Expression(left, right, type, 0);

        type = currentToken.tokenType;
        if (type == END) {
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