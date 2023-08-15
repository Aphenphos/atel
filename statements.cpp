#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"



void Statement::varDeclaration(void) {
    int id;

    Parse::nextToken();
    checkCurToken(IDENT);

    id = Symbols::addGsymbol((char*)prevToken.literal.string, prevToken.tokenType, VAR);
    Asm::globalSymbol(id);
    
    checkCurToken(SEMICOLON);
}

Expression* Statement::assignmentStatement(void) {
    Expression* left, *right, *tree;
    TokenType leftType, rightType;
    int id;

    checkCurToken(IDENT);

    if ((id = Symbols::findGlobalSymbol(prevToken.literal.string)) == -1) {
        printf("Symbol not found : ");
        handleSyntaxError();
    }

    right = Expression::castLeaf(LVIDENT, Symbols::symbolTable[id].type, id);

    checkCurToken(EQ);

    left = Expression::binaryExpression(0);

    leftType = left->type; rightType = right->type;
    if (!Types::compatible(leftType,rightType, true)) handleFatalError(cp"Type Error");

    if (leftType != EMPTY) left = Expression::castUnary(leftType, right->type, left, 0);

    tree = new Expression(left, nullptr, right, EQ , INT, 0);

    return tree;
}

Expression* Statement::printStatement(void) {
    Expression* tree;
    TokenType leftType, rightType;
    int r;

    checkCurToken(PRINT);

    tree = Expression::binaryExpression(0);

    leftType = INT;
    rightType = tree->type;
    if (!Types::compatible(leftType, &rightType, false)) handleFatalError(cp"Type Error");

    if (rightType != EMPTY)
        tree = Expression::castUnary(rightType, INT, tree, 0);
    
    tree = Expression::castUnary(PRINT, EMPTY, tree, 0);

    return tree;
}

Expression* Statement::singleStatement(void) {
    switch(currentToken.tokenType) {
        case PRINT:
            return printStatement();
        case CHAR:
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
                left = new Expression(left, nullptr, tree, HOLDER, EMPTY, 0);
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

    return new Expression( condAST, trueAST, falseAST, IF, EMPTY, 0);
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

    return new Expression(condAST, nullptr, bodyAST, WHILE, EMPTY, 0);

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

    tree = new Expression(bodyAST, nullptr, postAST, HOLDER, EMPTY, 0);

    tree = new Expression(condAST, nullptr, tree, WHILE, EMPTY,0);

    return new Expression(preAST, nullptr, tree, HOLDER, EMPTY, 0);


}


Expression* Statement::funcDeclaration(void) {
    Expression* tree;
    int nameSlot;

    checkCurToken(VOID);
    checkCurToken(IDENT);

    nameSlot = Symbols::addGsymbol((char*)prevToken.literal.string, VOID, FUNCTION);
    checkCurToken(LEFT_PAREN);
    checkCurToken(RIGHT_PAREN);

    tree = Statement::compoundStatement();

    return Expression::castUnary(FUNCTION, VOID, tree,  nameSlot);
}