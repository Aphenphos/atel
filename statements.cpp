#include <stdio.h>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"


int Statement::currentFuncID;

void Statement::varDeclaration(TokenType type) {
    int id;

    if (currentToken.tokenType == LEFT_BRACE) {

        if (Parse::peek().tokenType == INTLIT) {
            id = Symbols::addGsymbol(Parse::prev().literal.string, Types::pointer(type), ARRAY, 0, Parse::peek().literal.intLiteral);
            Asm::globalSymbol(id);
        }
        Parse::nextToken();
        Parse::nextToken();
        checkCurToken(RIGHT_BRACE);
    } else {
        id = Symbols::addGsymbol(Parse::prev().literal.string, type, VAR, 0, 1);
        Asm::globalSymbol(id);
    }
    checkCurToken(SEMICOLON);
} 

Expression* Statement::singleStatement(void) {
    switch(currentToken.tokenType) {
        case CHAR:
        case INT:
        case LONG:
            TokenType type;
            type = currentToken.tokenType;
            Parse::nextToken();
            Parse::nextToken();
            varDeclaration(type);
            return nullptr;
        case WHILE:
            return whileStatement();
        case IF:
            return ifStatement();
        case FOR:
            return forStatement();
        case RETURN:
            return returnStatement();
        default:
            return Expression::binaryExpression(0);
    }
}

Expression* Statement::compoundStatement(void) {
    Expression* left = nullptr;
    Expression* tree;

    checkCurToken(LEFT_CURL);

    while (1) {
        tree = singleStatement();

         if (tree != nullptr && (tree->op == RETURN || tree->op == EQ || tree->op == FUNCCALL)) {
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
    nameSlot = Symbols::addGsymbol(Parse::prev().literal.string, t, FUNCTION, endLabel, 0);

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
            Parse::genAST(tree, nl, TokenType(0));
        } else {
            varDeclaration(type);
        }

        if (currentToken.tokenType == END) {
            break;
        }
    }
}

Expression* Statement::accessArray(void) {
    Expression* left, *right;
    int id;

    if ((id = Symbols::findGlobalSymbol(Parse::prev().literal.string)) == -1 || Symbols::symbolTable[id].sType != ARRAY) {
        handleFatalError(cp"Undeclared Arr");
    }

    left = Expression::castLeaf(ADDRESS, Symbols::symbolTable[id].type, id);

    Parse::nextToken();

    right = Expression::binaryExpression(0);

    checkCurToken(RIGHT_BRACE);

    if (!Types::isInt(right->type)) {
        handleFatalError(cp"Array index not int");
    }

    right = Types::modifyType(right, left->type, PLUS);

    left = new Expression(left, nullptr, right, PLUS, Symbols::symbolTable[id].type, 0);
    left = Expression::castUnary(DEREF, Types::pointerValue(left->type), left, 0);
    return left;
}