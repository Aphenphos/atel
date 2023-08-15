#include <vector>
#include <iostream>
#include <cstring>
#include <map>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"

vector<Token> Parse::tokens;
int Parse::current;
int Parse::labelCount = 1;

map<TokenType, int> Expression::opPrecValues = 
{   
    {END , 0}, 
    {INTLIT, 0}, 
    {PLUS, 10}, 
    {MINUS, 10}, 
    {STAR, 20}, 
    {SLASH, 20},
    {EQ_EQ, 30},
    {BANG_EQ, 30},
    {LESS, 40},
    {LESS_EQ, 40},
    {GREAT, 40},
    {GREAT_EQ, 40}
};

Expression::Expression(Expression* pleft, Expression* pmid,  Expression* pright, TokenType pop, TokenType ptype, int pintValue) {
    left = pleft;
    middle = pmid;
    right = pright;
    op = pop;
    type = ptype;
    value.intValue = pintValue;
}

int Parse::label(void) {
    return(labelCount++);
}

void Parse::initParser(vector<Token>* ts) {
    cout << "_________Parser____________"<<endl;
    Parse::tokens = *ts;
    Parse::current = 0;
    currentToken = tokens[current];
}

void Parse::nextToken(void) {
    current++;
    currentToken = tokens[current];
}

int Parse::whileAST(Expression* n) {
    int lstart, lend;
    

    lstart = label();
    lend = label();
    Asm::label(lstart);

    genAST(n->left, lend, n->op);
    Asm::freeAllRegisters();

    genAST(n->right, nr, n->op);
    Asm::freeAllRegisters();

    Asm::jump(lstart);
    Asm::label(lend);
    
    return -1;
}

int Parse::genAST(Expression* n, int r, TokenType parent) {
    int leftRegister, rightRegister;

    switch(n->op) {
        case IF:
            return ifAST(n);
        case WHILE:
            return whileAST(n);
        case FUNCTION:
            Asm::funcPreamble(Symbols::symbolTable[n->value.id].name);
            genAST(n->left, nr, n->op);
            Asm::funcPostamble();
            return nr;
        case HOLDER:
            genAST(n->left , nr, n->op );
            Asm::freeAllRegisters();
            genAST(n->right, -1, n->op);
            Asm::freeAllRegisters();
            return nr;
    }

    if (n->left) {
        leftRegister = Parse::genAST(n->left, -1, n->op);
    }
    if (n->right) {
        rightRegister = Parse::genAST(n->right, leftRegister, n->op);
    }
    
    switch(n->op) {
        case PLUS:
            return Asm::add(leftRegister, rightRegister);
        case MINUS:
            return Asm::subtract(leftRegister, rightRegister);
        case STAR:
            return Asm::multiply(leftRegister, rightRegister);
        case SLASH:
            return Asm::divide(leftRegister, rightRegister);
        case INTLIT:
            return Asm::loadInt(n->value.intValue);
        case IDENT:
            return Asm::loadGlobalSymbol(Symbols::symbolTable[n->value.id].name);
        case LVIDENT:
            return Asm::storeGlobalSymbol(r, Symbols::symbolTable[n->value.id].name);
        case EQ_EQ:
        case BANG_EQ:
        case LESS:   
        case LESS_EQ:
        case GREAT:
        case GREAT_EQ:
            if (parent == IF || parent == WHILE) {
                return Asm::compareAndJump(n->op, leftRegister, rightRegister, r);
            } else {
                return Asm::compareAndSet(n->op, leftRegister, rightRegister);
            }
        case EQ:
            return(rightRegister);
        case PRINT:
            Asm::printInt(leftRegister);
            Asm::freeAllRegisters();
            return nr;
        case WIDEN:
            return Asm::widen(leftRegister);
        default:
            fprintf(stderr, "Parsing error while interpreting %d\0\n", n->op);
            exit(1);
    }

}

int Parse::ifAST(Expression* n) {
    int lfalse, lend;

    lfalse = label();

    if (n->right) {
        lend = label();
    }

    genAST(n->left, lfalse, n->op);
    Asm::freeAllRegisters();

    genAST(n->middle, -1, n->op);
    Asm::freeAllRegisters();

    if (n->right) {
        Asm::jump(lend);
    }

    Asm::label(lfalse);

    if (n->right) {
        genAST(n->right, nr, n->op);
        Asm::freeAllRegisters();
        Asm::label(lend);
    }

    return -1;

}
