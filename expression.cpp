#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"


Expression* Expression::castLeaf(TokenType op, TokenType type, int intValue) {
    return(new Expression(nullptr, nullptr, nullptr, op, type, intValue));
}

Expression* Expression::castUnary(TokenType op, TokenType type, Expression* left, int intValue) {
    return (new Expression(left, nullptr, nullptr,  op, type, intValue));
}

Expression* Expression::castPrimary(void) {
    Expression* node;
    int id;
    switch(currentToken.tokenType) {
        case INTLIT:
            if ((currentToken.literal.intLiteral >= 0) && (currentToken.literal.intLiteral < 256))
                node = castLeaf(INTLIT, INT, currentToken.literal.intLiteral);
            else 
                node = castLeaf(INTLIT, INT, currentToken.literal.intLiteral);
            break;
        case IDENT:

            if (Parse::peek().tokenType == LEFT_PAREN) {
                return Statement::callFunction();
            }
            id = Symbols::findGlobalSymbol(currentToken.literal.string);
            if (id == -1) {
                handleUnknownVar();
            }
            node = castLeaf(IDENT, Symbols::symbolTable[id].type, id);
            break;
        default:
            fprintf(stderr, "Parsing error %d", currentToken.tokenType);
            exit(1);
    }
    Parse::nextToken();
    return node;
}

Expression* Expression::binaryExpression(int prevTokenPrec) {
    Expression* left, *right;
    TokenType type, leftType, rightType;

    left = castPrimary();
    type = currentToken.tokenType;

    if (type == SEMICOLON || type == RIGHT_PAREN) {
        return(left);
    }

    while(getOpPrec(type) > prevTokenPrec) {
        Parse::nextToken();
        right = binaryExpression(opPrecValues.at(type));

        leftType = left->type;
        rightType = right->type;
        if (!Types::compatible(leftType, rightType, false)) handleFatalError(cp"Type Err");
        //may cause problem
        if (leftType != EMPTY) {
            left = castUnary(leftType, right->type, left, 0);
        }

        if (rightType != EMPTY) {
            right = castUnary(leftType, left->type, right, 0);
        }

        left = new Expression(left, nullptr, right, type, left->type, 0);

        type = currentToken.tokenType;

        if (type == SEMICOLON || type == RIGHT_PAREN) {
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
    int errorLine = Scan::currentLine;
    if (prec == 0) {
        fprintf(stderr, "syntax error on line:%d token: %d", errorLine, type);
        exit(1);
    }

    return prec;
}
