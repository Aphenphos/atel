#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

Expression::Expression(Expression* pleft, Expression* pmid,  Expression* pright, TokenType pop, TokenType ptype, int pintValue) {
    left = pleft;
    middle = pmid;
    right = pright;
    op = pop;
    type = ptype;
    value.intValue = pintValue;
}

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
                Parse::nextToken();
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
    Expression* left, *right, *lTemp, *rTemp;
    TokenType type, leftType, rightType, astOp;

    left = prefix();
    type = currentToken.tokenType;

    if (type == SEMICOLON || type == RIGHT_PAREN) {
        left->r=1; return left;
    }

    while(getOpPrec(type) > prevTokenPrec || (Expression::rightAssoc(type) == prevTokenPrec && getOpPrec(type) == prevTokenPrec)) {
        Parse::nextToken();
        right = binaryExpression(opPrecValues.at(type));

        astOp = type;

        if (astOp == EQ) {
            right->r = 1;
            right = Types::modifyType(right, left->type, EMPTY);
            if (left == nullptr) {
                printf("incompatable expression in assignment");
                exit(1);
            }

            lTemp = left; left = right; right = lTemp;
        } else {
            left->r = 1;
            right->r = 1;
            
            lTemp = Types::modifyType(left, right->type, astOp);
            rTemp = Types::modifyType(right, left->type, astOp);

            if (lTemp == nullptr && rTemp == nullptr) { handleFatalError(cp"BinaryExpr error"); }
            if (lTemp != nullptr) { left = lTemp;}
            if (rTemp != nullptr) { right = rTemp;}
        }

        left = new Expression(left, nullptr, right, astOp, left->type, 0);

        type = currentToken.tokenType;
        if (type == SEMICOLON || type == RIGHT_PAREN) { left->r=1; return left;}
    }

    left->r=1 ;return left;
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

Expression* Expression::prefix(void) {
    Expression* tree;
    switch(currentToken.tokenType) {
        case REF: {
            Parse::nextToken();
            tree = prefix();

            if (tree->op != IDENT) {
                handleFatalError(cp"& must be followed by an ident");
            }

            tree->op = ADDRESS;
            tree->type = Types::pointer(tree->type);
            break;
        }
        case STAR: {
            Parse::nextToken();
            tree = prefix();

            if (tree->op != IDENT && tree->op != DEREF) {
                handleFatalError(cp"* must be followed by an ident");
            }

            tree = castUnary(DEREF, Types::pointerValue(tree->type), tree, 0);
            break;
        }
        default:
            tree = castPrimary();
    }
    return tree;
}

int Expression::rightAssoc(TokenType type) {
    if (type == EQ) {
        return 1;
    }
    return 0;
}