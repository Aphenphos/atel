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
    {LONG, 8}
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