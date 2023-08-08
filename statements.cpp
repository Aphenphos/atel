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

    matchTokenAndString(INT, cp"int");
    matchTokenAndString(IDENT, cp"ident");

    Symbols::addGsymbol((char*)prevToken.literal.string);
    Asm::globalSymbol((char*)prevToken.literal.string);
    
    matchTokenAndString(SEMICOLON, cp";");
}

Expression* Statement::assignmentStatement(void) {
    Expression* left, *right, *tree;
    int id;

    matchTokenAndString(IDENT,cp"ident");

    if ((id = Symbols::findGlobalSymbol(prevToken.literal.string)) == -1) {
        printf("Symbol not found : ");
        handleSyntaxError();
    }

    right = Expression::castLeaf(LVIDENT, id);

    matchTokenAndString(EQ,cp "=");

    left = Expression::binaryExpression(0);

    tree = new Expression(left, nullptr, right,ASSIGN,0);

    matchTokenAndString(SEMICOLON,cp ";");

    return tree;
}

Expression* Statement::printStatement(void) {
    Expression* tree;
    int r;

    matchTokenAndString(PRINT,cp"print");

    tree = Expression::binaryExpression(0);

    tree = Expression::castUnary(PRINT,tree,0);

    matchTokenAndString(SEMICOLON,cp";");

    return tree;
}

Expression* Statement::compoundStatement(void) {
    Expression* left = nullptr;
    Expression* tree;

    matchTokenAndString(LEFT_CURL,cp"{");

    while (1) {
        switch(currentToken.tokenType) {
            case PRINT:
                tree = printStatement();
                break;
            case INT:
                varDeclaration();
                tree = nullptr;
                break;
            case IDENT:
                tree = assignmentStatement();
                break;
            case WHILE:
                tree = whileStatement();
                break;
            case IF:
                tree = ifStatement();
                break;
            case RIGHT_CURL:
                matchTokenAndString(RIGHT_CURL,cp"}");
                return left;
            default:
                handleSyntaxError();
        }

        if (tree) {
            if (left == nullptr) {
                left = tree;
            }
            else {
                left = new Expression(left,nullptr,tree, HOLDER, 0);
            }
        }
    }
}

Expression* Statement::ifStatement(void) {
    Expression* condAST, *trueAST, *falseAST = nullptr;

    matchTokenAndString(IF, cp"if");
    matchTokenAndString(LEFT_PAREN, cp"(");

    condAST = Expression::binaryExpression(0);

    //will need to optomimze
    switch(condAST->op) {
        case EQ_EQ:
        case BANG_EQ:
        case LESS:
        case LESS_EQ:
        case GREAT:
        case GREAT_EQ:
            break;
        default:
            handleFatalError(cp"Invalid conditional operator");
    }

    matchTokenAndString(RIGHT_PAREN, cp")");

    trueAST = compoundStatement();

    if (currentToken.tokenType == ELSE) {
        Parse::nextToken();
        falseAST = compoundStatement();
    }

    return new Expression( condAST, trueAST, falseAST, IF,  0);
}

Expression* Statement::whileStatement(void) {
    Expression* condAST, *bodyAST;

    matchTokenAndString(WHILE,cp"while");
    matchTokenAndString(LEFT_PAREN,cp"(");

    condAST = Expression::binaryExpression(0);

    //will need to optomimze
    switch(condAST->op) {
        case EQ_EQ:
        case BANG_EQ:
        case LESS:
        case LESS_EQ:
        case GREAT:
        case GREAT_EQ:
            break;
        default:
            handleFatalError(cp"Invalid conditional operator");
    }

    matchTokenAndString(RIGHT_PAREN,cp(")"));

    bodyAST = compoundStatement();

    return new Expression(condAST, nullptr, bodyAST, WHILE, 0);

}