#include <vector>
#include <iostream>
#include <cstring>
#include <map>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"



Expression::Expression(Expression* left, Expression* right, TokenType op, int intValue) {
    left = left;
    right = right;
    op = op;
    intValue = intValue;
}


void Parse::initParser(vector<Token> tokens) {
    Parse::tokens = tokens;
    Parse::current = 0;
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
        case INTLIT:
            return n->intValue;
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
            return node;
        default:
            handleSyntaxError();
            exit(1);
    }
}

Expression* Expression::binaryExpression(void) {
    Expression* node, *left, *right;
    TokenType type;
    left = castPrimary();

    if (currentToken.tokenType == END) {
        return(left);
    }

    type = currentToken.tokenType;

    Parse::nextToken();

    right = binaryExpression();
    
    node = new Expression(left,right,type,0);

    return node;
}

