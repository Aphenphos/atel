#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

void Statement::statements(void) {
    while (1) {
        switch(currentToken.tokenType) {
            case PRINT:
                printStatement();
                break;
            case INT:
                varDeclaration();
                break;
            case IDENT:
                assignmentStatement();
                break;
            case END:
                return;
            default:
                printf("invalid Statement : ");
                handleSyntaxError();
        }
        
    }
}

void Statement::varDeclaration(void) {

    matchTokenAndString(INT, (char*)"int");
    matchTokenAndString(IDENT, (char*)"ident");

    Symbols::addGsymbol((char*)prevToken.literal.string);
    Asm::globalSymbol((char*)prevToken.literal.string);
    
    matchTokenAndString(SEMICOLON, (char*)";");
}

void Statement::assignmentStatement(void) {
    Expression* left, *right, *tree;
    int id;

    matchTokenAndString(IDENT,(char*)"ident");

    if ((id = Symbols::findGlobalSymbol(prevToken.literal.string)) == -1) {
        printf("Symbol not found : ");
        handleSyntaxError();
    }

    right = Expression::castLeaf(LVIDENT, id);

    matchTokenAndString(EQ,(char*) "=");

    left = Expression::binaryExpression(0);

    tree = new Expression(left,right,ASSIGN,0);

    Parse::interpretAST(tree, -1);
    Asm::freeAllRegisters();
    matchTokenAndString(SEMICOLON,(char*) ";");
}

void Statement::printStatement(void) {
    Expression* tree;
    int r;

    matchTokenAndString(PRINT,(char*)"print");

    tree = Expression::binaryExpression(0);
    r = Parse::interpretAST(tree, -1);
    Asm::printInt(r);
    Asm::freeAllRegisters();

    matchTokenAndString(SEMICOLON,(char*)";");
}