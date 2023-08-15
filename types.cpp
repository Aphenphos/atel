#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

//THIS IS THE ERROR, PASSING BY REFERENCE IS NOT CHANGING THE VALUES
bool Types::compatible(TokenType &left, TokenType &right, bool o) {

    if ((left == VOID) || (right == VOID)) return false;

    if ((left == right)) left = right = EMPTY; return true;

    if ((left == CHAR) && (right == INT)) left = WIDEN; right = EMPTY; return true;

    if ((left == INT) && (right == CHAR)) {
        if (o) return false;
        left = EMPTY; right = WIDEN; return true;
    }

    left = right = EMPTY;

    return true;
}