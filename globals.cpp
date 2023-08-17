#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

void checkCurToken(TokenType type) {
    if (currentToken.tokenType == type) {
        Parse::nextToken();
    } else {

        printf("%d expected , got %d", type, currentToken.tokenType);
        exit(1);
    }
}