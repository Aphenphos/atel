#include <vector>
#include <iostream>
#include <cstring>
#include <map>

#include "atel.hpp"
#include "handleErr.hpp"
#include "defs.hpp"
#include "globals.hpp"


Expression Expression::equality() {
    Expression compare = comparison();
    while (Expression::match(BANG_EQ, EQ_EQ)) {
        Token op = prevToken();
        Expression right = comparison();
        compare = Binary(compare,op,right);
    }

    return compare;
}

Expression Expression::comparison() {
    Expression expression = additive();

    while (match(GREAT, GREAT_EQ, LESS, LESS_EQ)) {
        Token op = prevToken();
        Expression right = additive();
        expression = Binary(expression, op, right);
    }

}

bool Expression::match(TokenType types...) {
    for (int i = 0; i < ts.siz; i++) {
        if (check(ts.at(i)))
    }
}