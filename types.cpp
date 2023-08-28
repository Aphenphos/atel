#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

//will add more specific types like u8 etc
const map<TokenType, int> Types::size = {
    {EMPTY, 0},
    {VOID, 0},
    {CHAR, 1},
    {INT, 4},
    {LONG, 8},
    {LONGPTR, 8},
    {INTPTR, 8},
    {CHARPTR, 8}
};

bool Types::isInt(TokenType type) {
    if  (type == CHAR || type == INT || type == LONG) {
        return true;
    }
    return false;
}

bool Types::isPtr(TokenType type) {
    if (type == VOIDPTR || type == CHARPTR || type== LONGPTR || type == INTPTR) {
        return true;
    }
    return false;
}

bool Types::compatible(TokenType &left, TokenType &right, bool o) {
    int leftSize, rightSize;
    if (left == right) { left = right = EMPTY; return true; }
    leftSize = getSize(left);
    rightSize = getSize(right);

    if ((leftSize == 0) || (rightSize == 0))  {return false;}

    if (leftSize < rightSize) { left = WIDEN; right = EMPTY; return true; }

    if (rightSize < leftSize) {
        if (o) {return false;}
        left = EMPTY; right = WIDEN; 
        return true;
    }

    left = right = EMPTY;
    return true;
}

int Types::getSize(TokenType type) {
    return (size.at(type));
}

TokenType Types::pointer(TokenType type) {
    TokenType t;

    switch (type) {
        case VOID: t=VOIDPTR; break;
        case CHAR: t=CHARPTR; break;
        case INT: t=INTPTR; break;
        case LONG: t=LONGPTR; break;
        default:handleFatalError(cp"Unrecognized ptrType\n");
    }
    return t;
}

TokenType Types::pointerValue(TokenType type) {
    TokenType t;

    switch (type) {
        case VOIDPTR: t=VOID; break;
        case CHARPTR: t=CHAR; break;
        case INTPTR: t=INT; break;
        case LONGPTR: t=LONG; break;
        default:handleFatalError(cp"Unrecognized pointerValue\n");
    }

    return t;
}

TokenType Types::determine(void) {
    TokenType t;
    if (Parse::peek().tokenType == STAR) {
        t = pointer(currentToken.tokenType);
        Parse::nextToken();
        Parse::nextToken();
        return t;
    }
    t = currentToken.tokenType;
    Parse::nextToken();
    return t;
}


Expression* Types::modifyType(Expression* tree, TokenType rType, TokenType op) {
    TokenType lType;
    int lSize, rSize;

    lType = tree->type;

    if (isInt(lType) && isInt(rType)) {
        if (lType == rType) { return tree; }

        lSize = getSize(lType);
        rSize = getSize(rType);

        if (lSize > rSize) { return nullptr; }


        if (rSize > lSize) { return Expression::castUnary(WIDEN, rType, tree, 0); };
    }

    if (isPtr(lType)) {
        if (op == EMPTY && lType == rType) { return tree; }
    }


    if (op == PLUS || op == MINUS) {
        if (isInt(lType) && isPtr(rType)) {
            rSize = getSize(rType);
            if (rSize > 1) {
                return Expression::castUnary(SCALE, rType, tree, rSize);
            }
        }
    }

    return nullptr;
}