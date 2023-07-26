#include "atel.hpp"
#include "globals.hpp"


char nextChar(void) {
    char c;
    if (prevChar) {
        c = prevChar;
        prevChar = 0;
        return c;
    }
    c = fgetc(srcFile);
    if (c == '\n') {
        currentLine++;
    }

    return (c);
}