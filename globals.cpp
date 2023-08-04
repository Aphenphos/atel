#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"
//ERROR IS HERE NEED TO FIND A WAY TO EITHER NOT CONSUME TOKEN
//OR STORE ITS LITERAL;
void matchTokenAndString(TokenType type, char string[]) {
    if (currentToken.tokenType == type) {
        Parse::nextToken();
        return;
    } else {
        printf("%s expected on line %d", string, currentLine);
        exit(1);
    }
}