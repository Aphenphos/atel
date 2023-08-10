#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"



void Statement::varDeclaration(void) {

    checkCurToken(INT);
    checkCurToken(IDENT);

    Symbols::addGsymbol((char*)prevToken.literal.string);
    Asm::globalSymbol((char*)prevToken.literal.string);
    
    checkCurToken(SEMICOLON);
}

Expression* Statement::assignmentStatement(void) {
    Expression* left, *right, *tree;
    int id;

    checkCurToken(IDENT);

    if ((id = Symbols::findGlobalSymbol(prevToken.literal.string)) == -1) {
        printf("Symbol not found : ");
        handleSyntaxError();
    }

    right = Expression::castLeaf(LVIDENT, id);

    checkCurToken(EQ);

    left = Expression::binaryExpression(0);

    tree = new Expression(left, nullptr, right, EQ ,0);

    return tree;
}

Expression* Statement::printStatement(void) {
    Expression* tree;
    int r;

    checkCurToken(PRINT);

    tree = Expression::binaryExpression(0);

    tree = Expression::castUnary(PRINT,tree,0);

    return tree;
}

Expression* Statement::singleStatement(void) {
    switch(currentToken.tokenType) {
        case PRINT:
            return printStatement();
        case INT:
            varDeclaration();
            return nullptr;
        case IDENT:
            return assignmentStatement();
        case WHILE:
            return whileStatement();
        case IF:
            return ifStatement();
        case FOR:
            return forStatement();
        default:
            handleSyntaxError();
            exit(1);
    }
}

Expression* Statement::compoundStatement(void) {
    Expression* left = nullptr;
    Expression* tree;

    checkCurToken(LEFT_CURL);

    while (1) {
        tree = singleStatement();

         if (tree != nullptr && (tree->op == PRINT || tree->op == EQ)) {
            checkCurToken(SEMICOLON);   
         }


        if (tree != nullptr) {
            if (left == nullptr) {
                left = tree;
            } else {
                left = new Expression(left, nullptr, tree, HOLDER, 0);
            }
        }

        if (currentToken.tokenType == RIGHT_CURL) {
            checkCurToken(RIGHT_CURL);
            return left;
        }
    }
}

Expression* Statement::ifStatement(void) {
    Expression* condAST, *trueAST, *falseAST = nullptr;

    checkCurToken(IF);
    checkCurToken(LEFT_PAREN);

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

    checkCurToken(RIGHT_PAREN);

    trueAST = compoundStatement();

    if (currentToken.tokenType == ELSE) {
        Parse::nextToken();
        falseAST = compoundStatement();
    }

    return new Expression( condAST, trueAST, falseAST, IF,  0);
}

Expression* Statement::whileStatement(void) {
    Expression* condAST, *bodyAST;

    checkCurToken(WHILE);
    checkCurToken(LEFT_PAREN);

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

    checkCurToken(RIGHT_PAREN);

    bodyAST = compoundStatement();

    return new Expression(condAST, nullptr, bodyAST, WHILE, 0);

}

Expression* Statement::forStatement(void) {
    Expression *condAST, *bodyAST, *preAST, *postAST, *tree;

    checkCurToken(FOR);
    checkCurToken(LEFT_PAREN);

    preAST = singleStatement();
    checkCurToken(SEMICOLON);
    
    condAST = Expression::binaryExpression(0);
    
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
    checkCurToken(SEMICOLON);

    postAST = singleStatement();
    checkCurToken(RIGHT_PAREN);

    bodyAST = compoundStatement();

    tree = new Expression(bodyAST, nullptr, postAST, HOLDER, 0);

    tree = new Expression(condAST, nullptr, tree, WHILE, 0);

    return new Expression(preAST, nullptr, tree, HOLDER, 0);


}