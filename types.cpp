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
//THIS IS THE ERROR, PASSING BY REFERENCE IS NOT CHANGING THE VALUES
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

TokenType Types::pointer(void) {
    TokenType t;
    cout << currentToken.tokenType << endl;
    cout << Parse::prev().tokenType << endl;
    switch (currentToken.tokenType) {
        case VOID: t=VOIDPTR; break;
        case CHAR: t=CHARPTR; break;
        case INT: t=INTPTR; break;
        case LONG: t=LONGPTR; break;
        default:handleFatalError(cp"Unrecognized ptrType\n");
    }
    cout << "in ptr function" << t << endl;
    return t;
}

TokenType Types::pointerValue(void) {
    TokenType t;

    switch (currentToken.tokenType) {
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
    Parse::nextToken();
    if (currentToken.tokenType != STAR) {
        t = Parse::prev().tokenType;
        return t;
    };
    t = pointer();
    Parse::nextToken();
    return t;
}