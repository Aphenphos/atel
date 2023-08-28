#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"


int Statement::currentFuncID;

void Statement::varDeclaration(TokenType type) {
    int id;
    while (1) {
        id = Symbols::addGsymbol(Parse::prev().literal.string, type, VAR, 0);
        Asm::globalSymbol(id);
        if (currentToken.tokenType == SEMICOLON) {
            Parse::nextToken();
            return;
        }

        if (currentToken.tokenType == COMMA) {
            Parse::nextToken();
            checkCurToken(IDENT);
            continue;
        }
        handleFatalError(cp"Missing , or ;");
    }
}

Expression* Statement::assignmentStatement(void) {
    Expression* left, *right, *tree;
    int id;

    checkCurToken(IDENT);

    if (currentToken.tokenType == LEFT_PAREN) {
        return callFunction();
    }

    if ((id = Symbols::findGlobalSymbol(Parse::prev().literal.string)) == -1) {
        printf("Symbol not found : ");
        handleSyntaxError();
    }

    right = Expression::castLeaf(LVIDENT, Symbols::symbolTable[id].type, id);

    checkCurToken(EQ);

    left = Expression::binaryExpression(0);

    left = Types::modifyType(left, right->type, EMPTY);

    if (left == nullptr) handleFatalError(cp"Type Error");

    tree = new Expression(left, nullptr, right, EQ, INT, 0);

    return tree;
}

Expression* Statement::printStatement(void) {
    Expression* tree;

    checkCurToken(PRINT);

    tree = Expression::binaryExpression(0);

    tree = Types::modifyType(tree, INT, EMPTY);
    if (tree == nullptr) {
        handleFatalError(cp"Cant print");
    }


    tree = Expression::castUnary(PRINT, EMPTY, tree, 0);
    return tree;
}

Expression* Statement::singleStatement(void) {
    switch(currentToken.tokenType) {
        case PRINT:
            return printStatement();
        case CHAR:
        case INT:
        case LONG:
            TokenType type;
            type = currentToken.tokenType;
            Parse::nextToken();
            Parse::nextToken();
            varDeclaration(type);
            return nullptr;
        case IDENT:
            return assignmentStatement();
        case WHILE:
            return whileStatement();
        case IF:
            return ifStatement();
        case FOR:
            return forStatement();
        case RETURN:
            return returnStatement();
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


Expression* Statement::funcDeclaration(TokenType t) {
    Expression* tree, *finalStatement;
    int nameSlot, endLabel;
    endLabel = Parse::label();
    nameSlot = Symbols::addGsymbol(Parse::prev().literal.string, t, FUNCTION, endLabel);

    currentFuncID = nameSlot;

    //Will change when adding func parameters;
    checkCurToken(LEFT_PAREN);
    checkCurToken(RIGHT_PAREN);

    tree = compoundStatement();

    if (t != VOID) {
        finalStatement = (tree->op == HOLDER) ? tree->right : tree;
        if (finalStatement == nullptr || finalStatement->op != RETURN) {
            handleFatalError(cp"No return for non void function");
        }
    }

    return Expression::castUnary(FUNCTION, t, tree, nameSlot);
}

Expression* Statement::callFunction(void) {
    Expression* tree;
    int id;


    if ((id = Symbols::findGlobalSymbol(Parse::prev().literal.string)) == -1) {
        handleFatalError(cp"Undeclared Function");
    }
    checkCurToken(LEFT_PAREN);

    tree = Expression::binaryExpression(0);

    tree = Expression::castUnary(FUNCCALL, Symbols::symbolTable[id].type, tree, id);

    checkCurToken(RIGHT_PAREN);

    return tree;
}

Expression* Statement::returnStatement(void) {
    Expression* tree;
    TokenType returnType, funcType;

    if (Symbols::symbolTable[currentFuncID].type == VOID) {
        printf("Cant return from void func");
        exit(1);
    }

    checkCurToken(RETURN);
    checkCurToken(LEFT_PAREN);

    tree = Expression::binaryExpression(0);

    tree = Types::modifyType(tree, Symbols::symbolTable[currentFuncID].type, EMPTY);
    if (tree == nullptr) {
        handleFatalError(cp"Cant return");
    }

    tree = Expression::castUnary(RETURN, EMPTY, tree, 0);

    checkCurToken(RIGHT_PAREN);
    return tree;
}

void Statement::globalDeclarations(void) {
    Expression* tree;
    TokenType type;

    while (1) {
        type = Types::determine();
        checkCurToken(IDENT);

        if (currentToken.tokenType == LEFT_PAREN) {
            tree = funcDeclaration(type);
            Parse::genAST(tree, nr, TokenType(0));
        } else {
            varDeclaration(type);
        }

        if (currentToken.tokenType == END) {
            break;
        }
    }
}