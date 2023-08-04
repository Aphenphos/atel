#include <stdio.h>
#include <string.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"


int Symbols::globalSymbolsCount;
Symbols::Symbol Symbols::symbolTable[1024];

int Symbols::findGlobalSymbol(char* s) {
    int i;

    for (i = 0; i < globalSymbolsCount; i++) {
        if (*s == *symbolTable[i].name && !strcmp(s, symbolTable[i].name)) {
            return i;
        }
    }
    return -1;
}

int Symbols::addGsymbol(char* s) {
    int i;

    if ((i = findGlobalSymbol(s)) != -1) {
        return i;
    }

    i = newGlobalSymbol();
    symbolTable[i].name = strdup(s);
    return i;
}

int Symbols::newGlobalSymbol(void) {
    int i;

    if ((i = globalSymbolsCount++) >= 1024) {
        printf("Too many global symbols");
        exit(1);
    }

    return i;
}
