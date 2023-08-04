#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

void matchTokenAndString(TokenType type, char string[]) {
    if (currentToken.tokenType == type) {
        prevToken = currentToken;
        Parse::nextToken();
    } else {
        printf("%s expected on line %d", string, currentLine);
        exit(1);
    }
}